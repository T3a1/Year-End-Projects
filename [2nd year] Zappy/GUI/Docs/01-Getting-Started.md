# Zappy GUI — Getting Started

## 1. What this project is

Zappy GUI is the **graphical client** of the Zappy project. Zappy itself is a network multiplayer simulation: several AI clients connect to a central server and play tiny "trantorian" creatures trying to survive, gather resources, level up, and ultimately reach the maximum level through ritual incantations to win the game for their team.

The GUI does **not** run the game. It is a pure spectator / visualization tool:

- It connects to the Zappy **server** over TCP using the `GRAPHIC` protocol (the read-only observer protocol, as opposed to the protocol used by AI clients).
- It receives a stream of text commands describing the state of the world (map size, tiles, players, eggs, resources, incantations, broadcasts, end of game...).
- It turns that stream into a live 3D scene rendered with **raylib**: a textured ground, a population of animated 3D characters, eggs, resources scattered on tiles, and an in-game UI (player list, tile inspector, chat/broadcast log, settings book).

In short: the server is the brain, the AI clients are the players, and the GUI is the window through which a human can watch the game unfold in real time.

The client is written in **C++17**, built with **CMake**, and uses **raylib** as its only external dependency for windowing, 3D rendering, input, and audio.

---

## 2. Project layout — the GUI is one of three subprojects

This repository is a **monorepo** containing the three independent processes that make up the full Zappy system, each communicating with the others purely over TCP:

```
G-YEP-400-LIL-4-1-zappy-13/
├── Makefile          ← root orchestrator, builds everything below
├── NET/                ← the game server, written in C (binary: zappy_server)
├── GUI/                 ← the graphical client described in this document (C++ / raylib)
└── AI/                   ← the player client, written in Python (script: zappy_ai)
```

- **`NET`** is the authoritative server: it owns the world state (map, players, eggs, resources) and is the only process every other process talks to.
- **`GUI`** (this project) is a *read-only spectator* client: it connects to `NET` using the special `GRAPHIC` handshake and renders everything it's told about in 3D.
- **`AI`** is the player client: each Python process controls one in-game character and is what `GUI` actually displays moving around.

The GUI never talks to the AI directly, and vice versa — both only ever talk to the server. This document focuses on building and launching the **GUI**, but you need a running `zappy_server` (and, to see anything interesting, at least one `zappy_ai`) for the GUI to be useful at all — see §3.4 and §6.

## 3. Requirements

### 3.1 Operating system
The project targets Linux desktop environments with a graphical session (X11 or Wayland). It cannot run in a pure terminal/SSH session without a display — the program explicitly checks for this and refuses to start otherwise (see §5.3).

### 3.2 Build tools
- A C++17-capable compiler (g++ or clang++).
- **CMake** ≥ 3.10.
- `make` (the root `Makefile` drives the whole build, and itself invokes CMake for the GUI — see §4).

### 3.3 Runtime / graphics dependency
- **raylib**, found via CMake's `find_package(raylib REQUIRED)`.

You need raylib's development package installed and discoverable by CMake (either through your system package manager, a manually built/installed copy, or a CMake package registered via `find_package`).

Typical ways to get it:

- **From source (recommended if your distribution doesn't package a recent enough raylib):**
  ```
  git clone https://github.com/raysan5/raylib.git
  cd raylib
  mkdir build && cd build
  cmake .. -DBUILD_SHARED_LIBS=OFF
  make
  sudo make install
  ```
  This installs the headers and the static library system-wide so `find_package(raylib REQUIRED)` can locate it.

- **From a package manager**, if your distribution provides a sufficiently recent raylib package (the project does not pin an exact version, but it relies on modern raylib features such as 3D model + animation loading, `Camera3D`, and audio streaming, so an older raylib release may be missing symbols).

No other third-party library is required for the GUI itself: there is no separate networking library (raw POSIX sockets are used), no JSON/XML parser, and no audio middleware beyond what raylib already bundles.

### 3.4 A running Zappy server
The GUI is a client. It is useless on its own — you need a `zappy_server` instance already running and listening on a TCP port, with at least one team name configured, before you launch the GUI. It is built from the `NET/` subproject in this same repository (see §5.3) and is the process that actually generates the simulation the GUI displays.

---

## 4. Building the project

The repository is built from its **root**, through the top-level `Makefile`, not by `cd`-ing into `GUI/` and invoking CMake by hand. The root `Makefile` is simply an orchestrator that delegates to each subproject:

```makefile
zappy_server:
	@make -C NET

zappy_gui:
	@mkdir -p build
	@cmake -B build -S GUI
	@cmake --build build

zappy_ai:
	@cp AI/main.py zappy_ai
	@chmod +x zappy_ai
```

### 4.1 Building everything at once

From the repository root:

```
make all
```

This builds the server (`-C NET`), configures and builds the GUI out-of-source into a top-level `build/` directory, and stages the Python AI launcher script. At the end you get three runnable artifacts sitting at the repository root: `zappy_server`, `zappy_gui`, and `zappy_ai`.

### 4.2 Building only the GUI

```
make zappy_gui
```

This is equivalent to running, from the repository root:

```
cmake -B build -S GUI
cmake --build build
```

i.e. CMake is configured with `GUI/` as the **source** directory but `build/` (at the repository root) as the **binary/output** directory — this is an out-of-source build rooted one level above the `GUI/` folder, not inside it. Despite that, the resulting executable still ends up at the repository root next to `Assets/`, because `GUI/CMakeLists.txt` explicitly redirects the runtime output:

```cmake
set_target_properties(zappy_gui PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/..
)
```

Here `${CMAKE_SOURCE_DIR}` is `GUI/` (because that's the `-S` argument passed to `cmake -B build -S GUI`), so `${CMAKE_SOURCE_DIR}/..` resolves to the repository root — which is exactly where `Assets/` already lives. So after `make zappy_gui`, you'll find `./zappy_gui` at the repository root, while all of CMake's intermediate object files and caches live in `./build/`, not inside `GUI/`.

The compiler is invoked with `-Wall -Wextra` and the project enforces the **C++17** standard (`CMAKE_CXX_STANDARD 17`, `CMAKE_CXX_STANDARD_REQUIRED ON`, and `cxx_std_17` as a required compile feature on the target).

If you really want to invoke CMake manually instead of going through `make zappy_gui`, you must reproduce the same `-S GUI -B build` layout from the repository root — running `cmake ..` from inside a `GUI/build/` folder will *not* place the binary where the rest of the repo (and the `Assets/` folder) expects it.

### 4.3 Building only the server

```
make zappy_server
```

Delegates to `NET/Makefile`, which compiles the C sources (`CC = epiclang`, falling back to a standard C toolchain if unavailable) and places the `zappy_server` binary at the repository root (`NET/Makefile` sets `OUTDIR = ..` relative to `NET/`).

### 4.4 Cleaning

```
make clean      # removes build/ and NET's object files, leaves binaries
make fclean     # also removes the zappy_gui and zappy_ai binaries, and NET's binary/test artifacts
make re         # fclean + all
```

### 4.5 Assets
The GUI executable expects to find an `Assets/` folder next to it at runtime (textures, 3D models, fonts, music, sound effects, UI panels...). Because the binary is placed at the repository root by the build system, and `Assets/` already lives at the repository root, you don't need to copy anything manually — just run `./zappy_gui` from the repository root.

---

## 5. Launching the GUI

### 5.1 Usage summary

```
./zappy_gui -p port -h machine [-d density]
```

or, to only tweak the map decoration density:

```
./zappy_gui -d density
```

or to print usage help:

```
./zappy_gui --help
```

### 5.2 Flags and arguments

| Flag | Argument |  Description |
|------|----------|--------------|
| `-p` | `port` | TCP port of the Zappy server to connect to. Must be an integer between `0` and `65535`. |
| `-h` | `machine` |  IPv4 address of the machine running the Zappy server (e.g. `127.0.0.1`). Must be a valid dotted IPv4 address (4 numeric groups between 0 and 255, no malformed segments). Despite the flag name `-h` (a nod to the historical Zappy subject wording), this expects an **IP address**, not a hostname. |
| `-d` | `density` |  A float between `0.0` and `1.0` controlling how densely decorative props (rocks, trees, bushes) are scattered across the map. Purely cosmetic — it has no effect on gameplay logic, only on how cluttered the terrain looks. Defaults to `0.01` if not provided. |
| `--help` | — | Prints a one-line usage reminder and exits immediately (exit code `0`). Only valid as the sole argument. |

### 5.3 Accepted argument combinations

The internal argument parser (`Parser::parseArguments`) only accepts a small, strict set of shapes:

1. `./zappy_gui --help` → exactly 2 arguments, prints usage, returns immediately (the program exits before opening any window).
2. `./zappy_gui` → no arguments. This is allowed: it starts the program with default port/machine placeholders and the default map density; you would then typically fill in the connection details through the in-app menu (see §5).
3. `./zappy_gui -d 0.4` → 3 arguments, sets only the map density, leaves port/machine to be entered later in the menu.
4. `./zappy_gui -p 4242 -h 127.0.0.1` → 5 arguments, sets the port and machine directly.
5. `./zappy_gui -p 4242 -h 127.0.0.1 -d 0.4` → 7 arguments, sets port, machine, and density all at once.

Any other combination (wrong flag order, missing `-h` after `-p`, extra/missing arguments, malformed values) makes the program throw a `Zappy::GuiError` with an explanatory message and exit with status code **84** (the project's convention for "abnormal termination with a clean error message", used consistently across the Epitech Zappy ecosystem).

Concretely, the parser validates:
- That the port is a number within `[0, 65535]`.
- That the IP address has exactly 4 dot-separated, all-numeric segments, each within `[0, 255]`.
- That the density, if given, is a float within `[0.0, 1.0]`.

### 5.4 Graphical environment check
Before doing anything else, the parser checks that the program is being run inside a graphical session by inspecting the `DISPLAY` and `WAYLAND_DISPLAY` environment variables. If neither is present, the GUI refuses to start with a `GuiError("GUI must be run in a graphical environment")`. This is a deliberate safety net: raylib needs a window server to open a window, and trying to do so over a bare SSH terminal would otherwise crash with a much less friendly error.

### 5.5 What happens after argument parsing
Once arguments are accepted, the executable:

1. Seeds the random number generator (used later for decorative prop placement and team color generation).
2. Opens a fullscreen raylib window and initializes the audio device.
3. Shows the **in-game Menu** (see the architecture document) where you can review/correct the port and IP if needed, and actually trigger the connection attempt.
4. If you don't quit from the menu, it then constructs the **Core** (the main game object) with the chosen map density, opens the TCP connection to the server, sends the `GRAPHIC` handshake command, and enters the main loop.
5. The main loop keeps running until you close the window or trigger the in-app "quit" action; the audio device and the window are then cleanly released.

### 5.6 Exit codes
- `0`: normal termination (including `--help`).
- `84`: any caught `Zappy::GuiError` or generic `std::exception` (invalid arguments, network failure, missing graphical environment, asset loading failure, etc.). The error message is printed to `stderr` before exiting.

---

## 6. Quick start example

```
# Build everything from the repository root (server + GUI + AI)
make all

# 1. Start the server (must be running before the GUI can connect)
./zappy_server -p 4242 -x 20 -y 20 -n red blue -c 5 -t 10

# 2. In another terminal, launch the GUI, connecting straight away
./zappy_gui -p 4242 -h 127.0.0.1

# Or run with a denser decorative map
./zappy_gui -p 4242 -h 127.0.0.1 -d 0.5

# Or just launch and fill in the connection info from the in-game menu
./zappy_gui

# 3. (Optional) launch one or more AI players so there is something to watch
./zappy_ai -p 4242 -h 127.0.0.1 -n red
```

If the connection fails (wrong port, server not started, firewall, etc.), the GUI will show a "Connecting to server..." waiting screen and keep retrying network reads on every frame rather than crashing outright; a hard failure at the socket level (e.g. the initial `connect()` call failing) is reported as a `NetworkError` and the program exits with code `84`.
