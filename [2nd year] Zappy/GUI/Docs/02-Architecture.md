# Zappy GUI — Architecture & Internal Design

This document explains how the GUI client is structured internally: which modules exist, what each one is responsible for, and how data flows from a raw TCP byte stream to a rendered 3D frame. The goal is to give a precise mental map of the codebase so that anyone — including someone who never opened the project — can understand *why* the code is organized this way.

---

## 1. Bird's-eye view

The program is organized into clearly separated layers, each living in its own folder under `src/`:

```
main.cpp            -> entry point, argument parsing, top-level program flow
Parser/              -> command-line argument parsing & validation
Menu/                -> pre-game connection screen (port/IP entry)
Core/                -> the "game object": owns everything, runs the main loop
Network/              -> TCP socket + Zappy GRAPHIC protocol decoding
World/                -> game-world data: map, tiles, entities (players, eggs, props, resources)
Graphics/             -> everything related to drawing: raylib wrappers, 3D renderer, UI system
MusicManager/         -> background music playlist management
Utils/                -> shared constants and exception hierarchy
```

The dependency direction is, broadly:

```
main.cpp
   └── Parser           (no dependency on anything else)
   └── Menu              (uses Graphics/UI + Network, to let the user attempt a connection)
   └── Core               (the orchestrator)
         ├── Network        (Client + ProtocolHandler)
         ├── World           (Map, Tile, Player, Egg, Food, Stone, Prop)
         ├── Graphics         (Renderer, ViewCamera, UI subwindows/components)
         ├── MusicManager
         └── Core/ActionMap + KeyHandler (input → action indirection)
```

Nothing in `World/` or `Network/` knows about `Graphics/` — the dependency only goes one way, from the orchestrator (`Core`) down into the data layer and the presentation layer. This separation is the backbone of the whole design and is discussed in depth in the third document (good C++ practices), but it's worth keeping in mind while reading the rest of this one.

---

## 2. `main.cpp` — the program's skeleton

`main()` is intentionally thin. Its only responsibilities are:

1. Parse and validate command-line arguments through `Parser`.
2. Initialize raylib's window and audio device.
3. Show the `Menu` and wait for the user to either quit or provide valid connection details.
4. Construct a `Core` instance, connect it to the server, and call `core.run()` — which blocks until the program should exit.
5. Tear down audio/window cleanly and translate any uncaught `Zappy::GuiError` / `std::exception` into a clean error message + exit code `84`.

Two design choices are worth noting:

- **Scoped lifetimes via `{ }` blocks.** The `Menu` and the `Core` are each created inside their own braces. This guarantees their destructors run (releasing GPU resources such as textures and models) *before* `CloseWindow()` is called. Calling raylib's unload functions after the OpenGL context has been destroyed would be undefined behavior, so this ordering is not a style preference — it is a correctness requirement.
- **A single try/catch at the very top.** Every fallible operation in the program (parsing, socket connection, file loading) throws a typed exception instead of returning an error code. `main` is the single place that decides what an error means for the process (print to `stderr`, exit `84`). This keeps every other function's signature clean (no `bool` / `error_code` out-parameters polluting return types).

---

## 3. `Parser` — turning `argv` into validated configuration

`Parser` is a small, self-contained class with one real job: `parseArguments(argc, argv)`. It is deliberately *not* a generic command-line parsing framework — it hard-codes the handful of accepted argument shapes for this specific program (`--help`, no args, `-d`, `-p/-h`, `-p/-h/-d`). For each shape, it validates types and ranges immediately (port range, IPv4 format, density range) and throws a `GuiError` subtype with a human-readable message the moment something is wrong, rather than passing bad data downstream. This means every other module can assume the port/IP/density it's eventually handed are already valid — `Core` never has to re-check them.

---

## 4. `Menu` — the pre-game screen

Before the simulation starts, the user lands in `Menu`. It is a self-contained, fully separate "mini program" reusing the same UI/rendering building blocks as the main game (`SubWindowManager`, `Button`, `Renderer`, `ActionMap`) but with its own dedicated loop (`Menu::display()`), its own background music, and its own decorative animation (a little walking "Rat" sprite, handled through the `Rat` helper struct).

Its responsibilities:

- Display two editable text fields (port, IP) pre-filled with whatever was passed on the command line (or the placeholder `"X"` if nothing was passed).
- Let the user click into a field and type a new value (`editTextField`).
- Validate the typed values and attempt an actual TCP handshake (`tryToConnect`) when the user confirms, giving immediate visual feedback (a lock icon / color change) if the values are invalid or the connection fails.
- Expose the final, validated `port`/`machine` strings back to `main()`, plus a `shouldQuit()` flag if the user closed the window instead of confirming.

Architecturally, the menu matters because it shows the same component system (subwindows, buttons, action map) being reused in a completely different context from the main game — a sign that the UI layer is generic rather than hard-wired to one screen.

---

## 5. `Core` — the orchestrator

`Core` is intentionally the biggest class in the project, and that is by design, not by accident: it is the **composition root**. Every long-lived subsystem (camera, renderer, action map, key handler, subwindow manager, network client, protocol handler, music manager, model cache, map, players, eggs, panels) is owned — as a value, a `unique_ptr`, or a reference — directly inside `Core`. Nothing in the rest of the codebase needs a global, a singleton, or a static instance to reach these subsystems: they are explicitly passed down (by reference or via small lambdas) to whoever needs them. This is what makes the dependency graph traceable just by reading constructors.

### 5.1 What `Core` actually does, conceptually

Three responsibilities, cleanly separated into different sets of private methods:

1. **Bootstrapping** (`initSubWindows`, `initSubWindowsContent`, `initButtons`, `initPlayers`, `registerActions`): build every UI panel, every button, wire every keyboard/mouse action to a callback, and pre-load the player animation/texture caches. This all happens once, in the constructor.
2. **The main loop** (`run`, `update`, `draw`): described in detail in §5.3 below.
3. **Server-event handlers** (`onMsz`, `onBct`, `onPnw`, `onPpo`, ... one method per Zappy protocol command): described in §6. These are the methods `ProtocolHandler` calls once it has decoded a line from the network into structured data. `Core` is where that decoded data actually mutates the world (spawns a player, moves it, updates its inventory, etc).

### 5.2 Action-based input, not hard-wired input

Rather than checking `IsKeyPressed(KEY_X)` scattered across the codebase, every interactive element (a button, a key shortcut) is associated with a **named action string** ("togglePlayers", "toggleGrid", "closeProgram"...) through the `ActionMap` class:

```cpp
_actionMap.registerAction("toggleGrid", [this]() { _showGrid = !_showGrid; });
```

`KeyHandler` reads raw raylib input each frame and, instead of executing game logic itself, simply calls `_actionMap.trigger("someAction")`. Buttons do the same thing when clicked. This indirection means:

- The same action can be triggered by a key *and* a button without duplicating logic.
- `KeyHandler` and `Button` never need to know what `Core` actually does in response — they only deal with strings.
- Adding a new shortcut is a two-line change: register the action in `Core::registerActions`, and bind a key to it in `KeyHandler` (or a button to it via `Button`'s constructor).

### 5.3 The main loop

`Core::run()` is the classic real-time application loop:

```cpp
while (!WindowShouldClose() && !_shouldClose) {
    float deltaTime = GetFrameTime();
    if (_client) _client->update();        // 1. drain the socket, dispatch protocol events
    checkSetupCamera();                     // 2. lazily create the camera once map size is known
    if (no camera yet / protocol not ready) { drawWaitingScreen(); continue; }
    update(deltaTime);                      // 3. simulate
    draw();                                  // 4. render
}
```

Step by step:

1. **Network polling.** `_client->update()` does a non-blocking `poll()`/`recv()` on the socket, accumulates bytes into a line buffer, and for every complete line found, dispatches it to whichever handler `ProtocolHandler` registered for that command. This happens at the very top of every single frame — the simulation is effectively *driven* by the server, the GUI never invents game state on its own.
2. **Lazy camera setup.** The 3D camera can't be created until the map dimensions are known (`msz` must have arrived from the server first). `checkSetupCamera()` checks this every frame and creates the camera exactly once, centered on the map.
3. **Waiting screen.** Until the camera exists *and* the protocol handler reports the map fully loaded (`isReady()`), the loop just shows a "Connecting to server..." message and `continue`s — no game logic runs, no input is processed, nothing crashes due to missing data.
4. **`update(deltaTime)`** advances everything that needs per-frame simulation:
   - Keyboard/mouse input via `_keyHandler.update(...)`.
   - Window-resize handling (`_subwindowManager.recomputeAllSubWindows()`), so all UI panels stay correctly positioned regardless of window size.
   - **A fixed-tick accumulator** (`_tickAccumulator`) converts the server's `_timeUnit` (ticks per second) into a frame-rate-independent timer; every 7 server ticks, the GUI proactively asks the server for a fresh `mct` (map content) sync. This is a small but important detail: the GUI re-synchronizes itself periodically instead of trusting incremental updates forever, which protects against state drift if a message were ever lost.
   - Iterating over every `Player`, advancing its animation/movement interpolation, and removing players whose death animation has fully played.
   - Iterating over every `Tile` to make floating resource items bob up and down (`makeItemsFloat`), a purely cosmetic touch.
   - Delegating UI input handling (`processUI`) and 3D-picking (`handleTileClicks`, ray-casting from the mouse into the 3D scene to detect a clicked player or tile).
5. **`draw()`** does the actual rendering: 3D scene first (ground, grid, selected-tile highlight, players, eggs) inside a `BeginMode3D`/`EndMode3D` block, then 2D UI panels on top (`drawPanels()`), then a victory banner if a team has won.

This loop is a textbook **game loop** pattern: poll input/network → fixed-step simulate → render, all driven by raylib's `GetFrameTime()`.

---

## 6. `Network` — talking to the server

This layer is split into two classes with a single, sharp responsibility each:

### 6.1 `Client` — raw transport

`Client` knows about sockets, `poll()`, non-blocking I/O, and line-buffering. It does **not** know anything about what the lines it receives *mean*. Its public surface is deliberately minimal:

```cpp
bool connect();
void disconnect();
void send(const std::string &command);
void update();
void registerHandler(const std::string& command, std::function<void(const std::vector<std::string>&)> handler);
```

Internally, `update()` performs a non-blocking poll of the socket, appends any newly received bytes to an internal buffer, then repeatedly extracts `\n`-terminated lines and calls `parseLine()` on each one. `parseLine` only does the bare minimum of generic parsing: split on whitespace into a command word and a vector of string arguments, then look the command word up in a `std::unordered_map<std::string, std::function<...>>` of handlers and call it if found.

This handler-registration pattern means `Client` has **zero knowledge of the Zappy protocol's vocabulary** (`msz`, `bct`, `pnw`, etc.) — it is a generic "line-oriented TCP socket with a pluggable dispatch table." It could be reused as-is for a completely different text protocol.

### 6.2 `ProtocolHandler` — protocol semantics

`ProtocolHandler` is the piece that actually understands the Zappy `GRAPHIC` protocol. In its constructor, it registers one lambda per known server command with the `Client`:

```cpp
_client.registerHandler("msz", [this](const auto& args) { handleMsz(args); });
_client.registerHandler("bct", [this](const auto& args) { handleBct(args); });
...
```

Each `handleXxx` method follows the same defensive pattern:

1. Check the argument count matches what the command expects; log and bail out on mismatch.
2. Parse each argument from `std::string` to the expected type (`int`, sometimes stripping a leading `#` for IDs) using small helpers (`safe_stoi`, `parseId`) that return success/failure rather than throwing — a malformed line from the network is an *expected*, recoverable situation, not an exceptional one.
3. Validate semantic constraints (coordinates non-negative and in-bounds, orientation in `[1,4]`, quantities non-negative, etc.).
4. Only if everything checks out, call the corresponding `Core::onXxx(...)` method with clean, typed data.

`ProtocolHandler` also tracks two pieces of protocol-level state that don't belong to any single message: `_mapInitialized` (have we received `msz` yet) and `_mapFullyLoaded` (have we received as many `bct` tile-content lines as the map has tiles, so the world is fully populated before we start rendering it). These flags are exactly what `Core::run()` checks before leaving the waiting screen.

The separation between `Client` and `ProtocolHandler` is a clean example of **separation of concerns**: transport vs. semantics. You could swap the transport for, say, a buffered file replay (for a "replay an old game log" feature) without touching a single line of `ProtocolHandler`, as long as the replacement still calls `registerHandler`/feeds lines in the same way.

---

## 7. `World` — the simulated game state

### 7.1 `Map` and `Tile`

`Map` owns the grid dimensions, the ground 3D model, a procedurally-scattered set of decorative `Prop`s (rocks, trees, bushes — purely cosmetic, randomized using the `-d` density parameter), and a `std::vector<std::unique_ptr<Tile>>` representing every cell of the grid.

`Tile` owns what's *actually* gameplay-relevant for that cell: a list of `Food` items and a list of `Stone` items (one per resource type, see `StoneType`), plus counts. `Map::resize()` is called once, when the `msz` server message arrives, to (re)allocate the tile grid at the right size.

### 7.2 Entities: `IEntity` / `AEntity` and concrete entities

Every visible "thing" placed in the 3D world (a `Player`, an `Egg`, a `Food` item, a `Stone`, a decorative `Prop`) shares one contract, the `IEntity` interface: position, rotation, scale, color, an underlying raylib `Model`, distance-to functions, and a "is this model loaded yet" flag. `AEntity` is the shared implementation of that contract (storing the transform, owning the `Model` reference from the cache, implementing distance math), and every concrete entity inherits from it instead of re-implementing the same bookkeeping.

`Player` is the most elaborate entity. On top of the `AEntity` base it adds everything specific to a Zappy avatar: ID, team, level, inventory (`std::array<int, RESOURCE_COUNT>`), tile-to-tile movement interpolation (`_startPosition`/`_targetPosition`/`_elapsed`/`_travelTime`), a state machine of named animations driven by `AnimationHandler` ("Run", "Idle", "Die", "Kick", "Ejected", "Incantation", "Win"...), death/fade-out timers, and a colored marker shown above the model for quick visual identification on a crowded map.

`Egg` is comparatively tiny: an egg just sits at a position, optionally with a team color once it's known to belong to a player, until it either hatches (`ebo`) or dies (`edi`).

### 7.3 `ModelCache`

3D model loading (`.glb` files) is one of the more expensive operations raylib performs. `ModelCache` is a simple `std::map<std::string, Model>` keyed by file path: the first entity that requests a given model path triggers a real disk load; every subsequent entity asking for the same path gets a reference to the already-loaded model. Combined with `AEntity`'s `shared` flag (an entity can opt out of sharing if it truly needs its own private copy of a model, e.g. to apply unique per-instance mesh edits), this keeps GPU memory and load times under control even with dozens of players and eggs on screen simultaneously. `ModelCache::clearCache()` is called once, at the very end of `Core::run()`'s loop, to unload every model exactly once.

---

## 8. `Graphics` — rendering and UI

### 8.1 Raylib wrappers (`Graphics/Raylib/...`)

Raw raylib types like `Texture`, `Font`, `Music` are C structs with manual `LoadXxx`/`UnloadXxx` lifetimes — easy to leak or double-free if handled directly all over the codebase. The project wraps each one in a small RAII class: `ImageTexture`, `TextFont`, `MusicSound`, `RectangleImage`, `ViewCamera`. Each wrapper loads its resource in the constructor and unloads it in the destructor, disables copying (to avoid two objects trying to unload the same GPU handle), and allows moving (transferring ownership cleanly). This is the same idea as `std::unique_ptr`, just applied to raylib's C-style handles. See the third document for why this matters.

### 8.2 `Renderer`

`Renderer` is a stateless-ish helper that knows *how* to draw a `Map`, a list of `IEntity` references, a selection highlight, a grid, and (in menu mode) the menu's own images/text/rat sprite. Crucially, `Renderer::drawEntities` takes `std::vector<std::reference_wrapper<const IEntity>>` — it draws through the interface, never through a concrete `Player` or `Egg` type. This is why `Core::draw()` can build one mixed list of "anything that is an `IEntity`" and hand it to the same drawing call regardless of whether the underlying object is a player, a starting egg, or a hatched egg.

### 8.3 The subwindow / component UI system

The in-game UI (player panel, tile panel, chat panel, the settings "book", buttons) is built from a small generic framework rather than ad-hoc per-screen drawing code:

- **`SubWindow`**: a rectangular panel defined in *normalized* coordinates (fractions of the screen), with a visible/hidden state, a layer/z-order, and an ordered collection of `ISubWindowComponent`s it owns and updates/draws each frame. It also knows how to animate itself sliding in/out of view (`togglePosition`).
- **`SubWindowManager`**: a named registry of `SubWindow`s (`"main"`, `"players"`, `"chat"`, `"info"`, `"tile"`). It is the only place that knows about *all* panels at once, which is what allows generic operations like "close every other panel except the one being opened" (`closeAllSubWindows`) or "find which window is on top under the mouse" (`getFirstWindowInLayerOrder`) — exactly the kind of bookkeeping you don't want duplicated per-panel.
- **`ISubWindowComponent` / `ASubWindowComponent`**: the contract every visual widget inside a subwindow implements — `update`, `draw`, hit-testing (`onMouse`), visibility, layer. `ASubWindowComponent` factors out the normalized-rectangle math shared by every component (`resolveRect`, `getAbsoluteRect`) so concrete components only implement what's actually specific to them.
- **Concrete components** (`TextureComponent`, `TextComponent`, `Button`, `LoopingComponent`, `OneshotComponent`, `ContainerComponent`, and the higher-level `PlayerInfoComponent`, `TileInfoComponent`, `ChatInfoComponent`) each add one specific visual behavior: a static image, a piece of text, a clickable button bound to an `ActionMap` action, a looping or one-shot sprite-sheet animation, a container that groups other components, or a panel-specific information readout.
- **Panel handlers** (`PlayerPanelHandler`, `TilePanelHandler`, `ChatPanelHandler`, `BookHandler`) sit one level above the raw component system: they are the glue that, given a `SubWindow`, populate it with the right combination of components and wire them to live data sources via small getter lambdas supplied by `Core` (e.g. "give me the currently selected player", "give me the currently selected tile"). This is where `Core` hands out *read-only views* into its own state without giving the UI direct mutable access to `Core`'s internals.

This component system means adding a brand-new panel (say, a leaderboard) is a matter of: create a `SubWindow` entry, write one new "XHandler" class that composes existing components (or a new component if something truly new is needed), and wire its action(s) into `ActionMap` — no changes required anywhere else.

### 8.4 `ViewCamera`

A thin wrapper around raylib's `Camera3D` adding Zappy-specific behavior: a "drone" free-fly mode and a "fixed/orbit" mode (`CameraMode`), rectangular movement bounds clamped to the map's extents, and a `resetToHalfway()` convenience used both on startup and when the user presses the reset-camera shortcut.

---

## 9. `MusicManager`

A small playlist manager: holds a `std::map<std::string, MusicSound>` of every loaded track, a shuffled playlist order, and a "currently playing" optional reference. `playRandomMusic()` and `update()` (called every frame from `Core::update`) implement an autoplay-next-track behavior so background music keeps looping through the whole session without any of the rest of the codebase needing to care about it. `playMusic("Macarena")` is explicitly triggered once a team wins (`onSeg`), swapping the ambient soundtrack for the victory tune.

---

## 10. `Utils` — shared foundations

- **`Constants.hpp`** centralizes every magic number and asset path used throughout the project (window size/title/FPS, camera distances, asset file paths for every texture/model/sound, `RESOURCE_COUNT`, etc.) so they aren't duplicated or hard-coded inline in a dozen different files.
- **`Errors.hpp`** defines the project's exception hierarchy, all deriving from a common `GuiError` (itself deriving from `std::exception`): `LoadingError`, `ParsingError`, `ValidationError`, `NotFoundError`, `NetworkError`. Every part of the codebase that can fail throws the *most specific* applicable type, while `main` only ever needs to catch the common base (plus a generic `std::exception` fallback) to handle every failure uniformly.

---

## 11. End-to-end example: how a player spawning looks, top to bottom

To tie everything together, here is the full path of a single server event, from raw bytes to a visible 3D character:

1. The server sends a line like `pnw 12 5 7 2 1 TeamRed\n` over the TCP socket.
2. `Client::update()` reads the bytes, finds the `\n`, extracts the line, splits it into `cmd = "pnw"` and `args = ["12","5","7","2","1","TeamRed"]`, and looks up `"pnw"` in its handler table — finding the lambda `ProtocolHandler` registered for it.
3. `ProtocolHandler::handlePnw(args)` validates argument count, parses each field into an `int`/`std::string`, validates ranges, and on success calls `_core.onPnw(id, x, y, orientation, level, team)` with clean typed data.
4. `Core::onPnw(...)` checks the coordinates are within the current map bounds, resolves (or generates) a color for that team via `getTeamColor`, and constructs a new `Player` directly inside the `_players` map (`try_emplace`), passing it everything it needs to exist: id, team, starting animation model path, spawn tile, initial rotation, scale, a reference to the `Map` (so it can query terrain later), a reference to the shared `AnimationHandler`, the current server time unit, and a reference to the shared `ModelCache`.
5. From the very next frame onward, that `Player` is part of `Core::update()`'s iteration (animation/movement progression) and `Core::draw()`'s entity list (built as `std::vector<std::reference_wrapper<const IEntity>>` and handed to `Renderer::drawEntities`), so it appears, idle-animated, at the right tile on screen — without `Renderer` ever needing to know it's specifically a `Player` rather than an `Egg`.

This trace illustrates the whole point of the layering described above: each layer (transport → protocol → world state → rendering) only ever talks to the layer directly below or above it through a narrow, well-defined interface, which is exactly what makes the next document's discussion of extensibility possible.
