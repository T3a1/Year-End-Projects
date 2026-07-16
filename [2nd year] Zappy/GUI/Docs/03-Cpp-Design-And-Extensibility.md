# Zappy GUI — C++ Design Quality & Extensibility

This document looks at the project from a different angle than the architecture document: not "what does each module do", but **"why was it built this way, and what does that buy us in terms of maintainability and future upgrades?"**

The short version: the codebase consistently applies a handful of well-known object-oriented and modern-C++ principles — encapsulation, interface segregation, RAII, composition over inheritance abuse, and dependency injection through references/lambdas instead of globals. None of this is exotic, but applying it *consistently* is exactly what makes a project survivable past the date it was handed in: new features tend to slot into an existing extension point instead of requiring a rewrite.

We'll go principle by principle, point to where it shows up in the code, and then show concretely what kind of future upgrade each one enables.

---

## 1. Encapsulation: hide the "how", expose the "what"

Almost every class in the project follows the same shape: private data members, a small public surface of getters/setters/behavior methods, and `private`/`protected` sections for anything that's an implementation detail.

A representative example is `Client` (the TCP layer): its public interface is four verbs — `connect`, `disconnect`, `send`, `update` — plus a way to plug in handlers. Nothing about *how* it manages the socket file descriptor, the non-blocking flags, or the internal line buffer leaks into its public interface. `ProtocolHandler`, `Core`, and everything else only ever sees "I can send a string, and I get called back when a line arrives" — never a raw `int` file descriptor or a `pollfd` struct.

Another good example is `ModelCache`: its only two public operations are "give me the model for this path" and "clear everything". The fact that it's backed by a `std::map<std::string, Model>` internally is invisible to every entity that uses it — which means the caching strategy (a `map`, an `unordered_map`, an LRU cache, whatever) could be swapped later without touching a single entity class.

**Why this matters for upgrades:** because the *interface* is small and stable, the *implementation* is free to change. If a future version of the project needed, say, to switch the network layer from raw POSIX sockets to a library like `asio`, only `Client`'s internals would need to change — `ProtocolHandler`, `Core`, and everything above it would be completely unaffected, because they were never coupled to sockets in the first place, only to `connect/send/update/registerHandler`.

---

## 2. Interfaces and abstract base classes: programming to a contract

The project uses two flavors of polymorphism deliberately, in two different places, for two different reasons.

### 2.1 Pure interfaces: `IEntity`

`IEntity` is a pure abstract class (every method is `= 0`): position, rotation, scale, color, the underlying raylib `Model`, distance computation. It defines *what it means to be a drawable thing in the 3D world* without saying anything about *how* that thing stores its data.

`Renderer::drawEntities` takes a `std::vector<std::reference_wrapper<const IEntity>>`. It has no idea whether the object behind that reference is a `Player`, an `Egg`, or some future entity type that doesn't exist yet — and it doesn't need to. This is the **Open/Closed Principle** in practice: `Renderer` is closed for modification (you will never need to edit `Renderer::drawEntities` to support a new entity type) but the set of entity types is open for extension.

### 2.2 Abstract base + shared implementation: `AEntity`

`AEntity` implements `IEntity` and provides the storage and math every concrete entity needs (transform fields, distance calculations, the `ModelCache` reference). Concrete classes (`Player`, `Egg`, `Food`, `Stone`, `Prop`) inherit from `AEntity`, not from `IEntity` directly, so they get all of that bookkeeping for free and only add what's *actually* specific to them. `Player` adds inventory, animation state, and movement interpolation; `Egg` adds almost nothing beyond an ID and a hatch/death lifecycle.

The same pattern repeats in the UI layer: `ISubWindowComponent` is the pure contract every widget must satisfy (`update`, `draw`, hit-testing, visibility), and `ASubWindowComponent` is the shared base implementing the normalized-rectangle math (`resolveRect`, `getAbsoluteRect`) that every concrete component (`Button`, `TextComponent`, `TextureComponent`, …) would otherwise have to duplicate.

**Why this matters for upgrades:** adding an entirely new kind of entity or UI widget is a matter of writing one new class that inherits from the right abstract base, implementing only what's genuinely new, and it automatically becomes usable everywhere the interface is expected — in render lists, in subwindow component lists, in tile inventories — with zero changes to existing code.

*Example upgrade this enables:* adding a new resource type rendered as a 3D model on the ground (say, a rare seasonal item) would mean writing a small class similar to `Stone`/`Food` that inherits from `AEntity`, and adding it to `Tile`'s storage — `Renderer`, `Core::draw()`, and the rest of the rendering pipeline would not need a single line changed, because they already operate purely in terms of `IEntity`.

---

## 3. RAII: resource lifetime tied to object lifetime

Raylib resources (`Texture`, `Font`, `Music`, `Model`) are C-style handles that must be manually unloaded (`UnloadTexture`, `UnloadFont`, ...). Handling that manually, scattered across the codebase, is exactly how resource leaks and double-frees happen in real projects. Instead, the project wraps every such resource in a small RAII class — `ImageTexture`, `TextFont`, `MusicSound`, `RectangleImage`, `ViewCamera`:

- The constructor loads the resource.
- The destructor unloads it — always, even if an exception is thrown somewhere else, even if the object is destroyed as part of unwinding a `vector`.
- The copy constructor and copy-assignment operator are explicitly deleted (`ImageTexture(const ImageTexture&) = delete;`), because copying a GPU handle by value would mean two C++ objects both believing they own (and will eventually free) the same underlying texture.
- Move semantics are implemented instead, transferring ownership and leaving the moved-from object in a harmless, empty state (`other._texture = {}`).

This is precisely the same idea as `std::unique_ptr`, just hand-rolled for a C API that doesn't speak C++ ownership semantics natively. `unique_ptr` itself is used pervasively for owned objects whose construction must be deferred or polymorphic (`std::unique_ptr<Egg>`, `std::unique_ptr<Tile>`, `std::unique_ptr<Map>`, `std::unique_ptr<Client>`, `std::unique_ptr<ProtocolHandler>`...).

**Why this matters for upgrades:** anyone adding a new kind of loadable asset (a new sound effect category, a new font for a new UI panel) has an established pattern to follow — wrap it the same way the existing assets are wrapped — and gets correct cleanup "for free," without needing to reason about exactly when and where to call an `UnloadXxx` function.

---

## 4. Composition and dependency injection over globals

There is no singleton, no global mutable state, and no "god object reached via a static getter" anywhere in this codebase. Every subsystem that a class needs is either:

- **owned directly** (`Core` owns its `Renderer`, `ActionMap`, `KeyHandler`, etc. as plain members), or
- **passed in by reference** at construction time (`Player` takes `Map&`, `AnimationHandler&`, `ModelCache&`; `ProtocolHandler` takes `Client&` and `Core&`), or
- **handed a small lambda** that captures just enough context to answer one specific question, without exposing the whole object (e.g. `PlayerPanelHandler::init` receives a `getPlayer` lambda from `Core` rather than a pointer to `Core` itself).

This last pattern — injecting a narrow lambda instead of a wide reference — deserves its own callout, because it's used repeatedly and deliberately:

```cpp
auto getPlayer = [this]() -> const Player* {
    if (_players.empty() || ...) return nullptr;
    ...
};
_playerPanel = std::make_unique<PlayerPanelHandler>(_actionMap);
_playerPanel->init(players, getPlayer);
```

`PlayerPanelHandler` never gets a `Core&`. It gets exactly two things: an `ActionMap&` (so its buttons can trigger actions) and a function that, when called, returns "whichever player is currently selected, or null." It cannot accidentally reach into `Core`'s network client, its map, or its other panels, because it was never given the ability to. This is a hand-rolled, very lightweight form of the **Dependency Inversion Principle**: high-level code (`Core`) decides what low-level code (`PlayerPanelHandler`) is allowed to see, by constructing narrow capability objects instead of handing out broad access.

**Why this matters for upgrades:** because nothing is reached through a global, every dependency a class has is visible directly in its constructor signature. There is never a hidden coupling discovered only at runtime. A new contributor — or a future you — can open any single header and know exactly what that class can and cannot touch, which makes refactoring safe: if `PlayerPanelHandler`'s capabilities ever need to grow, you change the lambda it's given, not its access level to all of `Core`.

---

## 5. The action-map indirection: decoupling input from behavior

`ActionMap` is a deliberately tiny class (`registerAction` + `trigger`, backed by an `unordered_map<string, function<void()>>`), but it's a textbook **Command pattern**: an "action" is a name, and the actual behavior behind that name is registered once, centrally, in `Core::registerActions()`.

Both `KeyHandler` (reading raw keyboard/mouse state) and `Button` (a UI widget) only ever know action *names*, never the behavior. This double indirection (input source → action name → behavior) means input sources and behaviors can vary independently:

- The same action ("togglePlayers") is currently reachable by both a keyboard shortcut and a clickable button, with zero duplicated logic.
- A new control scheme (gamepad support, for instance) could be added by writing a new input-reading class that also just calls `_actionMap.trigger(name)` — no existing action registration would need to change at all.

*Example upgrade this enables:* adding a "screenshot" feature is registering one new action (`_actionMap.registerAction("takeScreenshot", [](){ ... });`) and binding it to a key in `KeyHandler` and/or a new button — nothing else in `Core` needs to be touched, and there is no risk of breaking an existing shortcut.

---

## 6. A typed exception hierarchy instead of error codes

`Errors.hpp` defines `GuiError` (deriving from `std::exception`) and a handful of specific subtypes — `LoadingError`, `ParsingError`, `ValidationError`, `NotFoundError`, `NetworkError`. Every fallible operation across the codebase throws the most specific type that applies. `main()` only needs one `catch` clause for the common base (plus a fallback `catch (const std::exception&)`) to handle every possible failure uniformly, while still allowing any future code, if it wanted to, to `catch` a *specific* error type and react differently to it (e.g. retry on `NetworkError` but abort immediately on `ValidationError`).

**Why this matters for upgrades:** if a future version wanted to add, say, automatic reconnection on network failure, that's a matter of adding one new `catch (const NetworkError&)` block somewhere in the loop — the existing exception hierarchy already distinguishes "this failure was a network problem" from any other kind of failure, so no new error-classification logic is needed.

---

## 7. The protocol/handler dispatch table: adding new server commands safely

`Client::registerHandler` plus `ProtocolHandler`'s constructor form a **dispatch table** pattern: one entry per known server command, each mapped to a small parsing+validation function. The two responsibilities — "is this line syntactically well formed" and "what does this line mean for the simulation" — are split across `ProtocolHandler` (parsing/validation) and `Core` (the actual `onXxx` mutation of world state).

*Example upgrade this enables:* if the Zappy protocol ever gained a new server command (say, a hypothetical `wth` "weather" event), supporting it would be:
1. One new `registerHandler("wth", ...)` call in `ProtocolHandler`'s constructor.
2. One new `handleWth(args)` private method following the exact same validate-then-forward shape as every existing handler.
3. One new `onWth(...)` method on `Core` that actually does something with the data (e.g. tint the lighting, spawn a particle effect).

No existing handler needs to change, and the unknown-command fallback in `Client::parseLine` (which currently just logs to `stderr`) guarantees that an *unsupported* command never crashes the program — it's simply ignored with a warning until support for it is added.

---

## 8. The UI component system: a small framework, not a pile of one-off screens

As detailed in the architecture document, every visible UI element is a `ISubWindowComponent`, and every panel is a `SubWindow` holding a collection of them. This is effectively a miniature, purpose-built **GUI framework** living inside the project, rather than ad-hoc immediate-mode drawing calls duplicated per screen.

*Example upgrade this enables:* adding a brand-new panel — say, a scoreboard showing each team's resource totals — would mean:
1. Registering a new normalized `SubWindow` in `Core::initSubWindows`.
2. Writing one new "`ScoreboardHandler`" class, composing existing components (`TextComponent`, `ContainerComponent`) the same way `PlayerPanelHandler` or `TilePanelHandler` already do, fed by a small lambda from `Core` exposing whatever read-only data it needs.
3. Wiring one toggle action and, optionally, one button to open/close it.

None of `SubWindow`, `SubWindowManager`, the existing components, or `Renderer` would need to change, because the framework was built generically from the start rather than tailored to exactly the five panels that currently exist.

---

## 9. Const-correctness and read-only views

Throughout the codebase, methods that don't need to mutate state are marked `const` (`getWidth() const`, `getTeam() const`, `isVisible() const`, ...), and several places explicitly hand out `const` references or pointers to avoid letting a consumer accidentally mutate state it only needs to read — for instance, `TilePanelHandler` and `PlayerPanelHandler` are fed lambdas returning `const Tile*` / `const Player*`, not mutable pointers, and `Renderer::drawEntities` takes its entity list as `const IEntity&` references.

**Why this matters for upgrades:** this is a compiler-enforced guarantee, not a convention someone has to remember. If a future contributor tried to make the UI layer mutate gameplay state directly (instead of going through `ActionMap`/`Core`), it simply wouldn't compile — which keeps the "UI reads, Core decides" boundary from quietly eroding over time as the project grows.

---

## 10. Putting it together: why this design ages well

None of the individual techniques above are advanced. What makes the project genuinely easy to extend is that they are applied **consistently** across every layer:

- Need a new entity type? Inherit `AEntity`/`IEntity`. Rendering, distance checks, and caching already work.
- Need a new UI panel? Inherit/compose `ISubWindowComponent`s inside a new `SubWindow`. Input, layering, and animation already work.
- Need a new shortcut or button? Register one `ActionMap` action. Input dispatch already works.
- Need to support a new server message? Add one handler in `ProtocolHandler`, one `onXxx` in `Core`. Network plumbing already works.
- Need a new loadable asset type? Wrap it in a small RAII class like the existing ones. Lifetime management already works.

In every case, the "already works" part is exactly what encapsulation, interfaces, RAII, and dependency injection bought the project: a stable set of extension points that new code can plug into, instead of a monolith that has to be understood and risked breaking in its entirety for every new feature.
