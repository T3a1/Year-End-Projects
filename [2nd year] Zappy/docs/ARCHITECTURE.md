# Zappy Architecture

This document explains the overall system design, how the three components interact, and the principles behind key design decisions.

---

## 1. System Overview

Zappy is a **distributed system** with three independent processes communicating over TCP:

```
┌─────────────────────────────────────────────────────────────┐
│                        TCP (port N)                         │
│                                                             │
│   ┌───────────────┐        ┌──────────┐   ┌────────────┐    │
│   │  NET server   │◄──────►│  GUI     │   │  AI (×N)   │    │
│   │  (C, poll)    │◄──────►│ (C++/    │   │  (Python)  │    │
│   │               │        │ Raylib)  │   │            │    │
│   └───────────────┘        └──────────┘   └────────────┘    │
└─────────────────────────────────────────────────────────────┘
```

### Component Roles

| Component | Language | Role | Relationship |
|-----------|----------|------|--------------|
| **NET Server** | C | Authority | Single source of truth; owns all game state |
| **GUI Client** | C++/Raylib | Observer | Read-only spectator; renders live visualization |
| **AI Agents** | Python | Players | Connect as clients; send commands, receive responses |

**Key principle:** The server is the single source of truth. GUI and AI never talk directly—all communication flows through the server using a well-defined text protocol.

---

## 2. Server Architecture (NET)

### 2.1 Overall Design

The NET server is an **event-driven, single-threaded application** using `poll(2)` for I/O multiplexing:

```
main()
  ├─ init_server()          ← Allocate server_t, network_t, world_t
  └─ run_server()           ← Main loop
       └─ poll()            ← Block until next event or timeout
       ├─ handle_tick()     ← Fire tick-based events (food consumption, command execution)
       └─ handle_poll_events()
          ├─ new_connection()       ← Accept new clients
          ├─ receive_commands()     ← Read from sockets
          └─ execute_commands()     ← Dispatch to handlers
```

**Why event-driven?**
- No threads = no race conditions = deterministic behavior
- Easier to reason about timing (each tick is atomic)
- Scales well on LAN without thread overhead

### 2.2 Core Data Structures

The server maintains three main objects:

```
server_t
├── args_t              ← Command-line arguments (port, width, height, teams, etc.)
├── network_t           ← Socket file descriptors, connections linked list, blacklist
└── world_t             ← All game state (players, map, resources, commands)
    ├── tile_t *grid[][]    ← Flat [height][width] array, doubly-linked as torus
    ├── ai_t *ais           ← Linked list of live players/eggs
    ├── gui_t *guis         ← Linked list of spectator clients
    ├── team_t *teams       ← Linked list of teams and their players
    ├── command_t *queue    ← Global command queue (sorted by remaining_time)
    └── size_t resource_counts[7]  ← Global food/mineral counters
```

**Tile structure** — one cell of the map:
```c
typedef struct tile {
    int x, y;
    struct tile *north, *south, *east, *west;  // Torus topology: always valid neighbors
    tile_content_t *content;  // Resources and player pointers
} tile_t;
```

**AI structure** — a player or unhatched egg:
```c
typedef struct ai {
    size_t id;                // Unique player ID
    size_t level;             // 0=egg, 1–7=playing, 8=max
    team_t *team;
    direction_t direction;    // NORTH / EAST / SOUTH / WEST
    int inventory[7];         // 7 resource slots
    tile_t *position;         // Current tile
    command_t *local_commands; // Per-player FIFO queue
    time_t last_check;        // For food depletion timing
    bool is_incanting;        // Currently in ritual
    struct ai *next;
} ai_t;
```

**Connection structure** — one TCP client:
```c
typedef struct connection_s {
    int fd;                   // Socket file descriptor
    client_type_t type;       // GUI / AI / UNKNOWN
    client_data_t cli;        // Union: {ai_t *ai; gui_t *gui;}
    char *partial_command;    // TCP read buffer for incomplete lines
    size_t partial_len;
    struct connection_s *next;
} connection_t;
```

### 2.3 Command Execution Model

Every AI command goes through a two-queue system:

1. **Global queue** — all commands across all players, sorted by remaining time
2. **Per-player local queue** — up to 10 pipelined commands from one AI

**Execution flow:**

```
AI sends "Forward"
    │
    ▼
execute_commands()
    │
    ├─ Player already has command executing?
    │   YES → Append to local_commands (queue it)
    │   NO  → add to global queue immediately
    │
    ▼
refresh_commands() [every tick]
    │
    ├─ For each due command:
    │   ├─ Execute it
    │   ├─ Remove from global queue
    │   └─ Pop next from local_commands → add to global (deferred)
    │
    └─ Update next_command_time
```

This ensures each player has **exactly one command executing** at a time, while allowing pipelining up to 10 ahead.

### 2.4 Main Loop Timing

```
loop:
    poll(timeout = next_command_time)
    │
    ├─ timeout (ret == 0): tick fired, no I/O
    ├─ ret > 0: I/O events + possible tick
    └─ ret < 0: signal or error, retry
    │
    ├─ handle_tick()
    │  ├─ refresh_ais()      ← Check for food depletion / death
    │  ├─ refresh_commands() ← Execute due commands
    │  └─ win()              ← Check victory condition
    │
    └─ handle_poll_events()
       ├─ new_connection()    ← accept() + WELCOME
       ├─ receive_commands() ← recv() + buffer management
       └─ execute_commands() ← Dispatch to handlers
```

---

## 3. GUI Architecture (C++)

### 3.1 Layered Design

The GUI is organized into clearly separated layers:

```
main.cpp             → Entry point, initialization, top-level exception handling
Parser/              → Command-line argument validation
Menu/                → Pre-game connection screen
Core/                → Orchestrator: owns all subsystems, runs main loop
  ├── Network/       → TCP socket + protocol decoding
  ├── World/         → Game state: Map, Tiles, Players, Eggs, Food
  ├── Graphics/      → Rendering: Camera, UI, Animations
  └── MusicManager/  → Background audio playlist
```

**Key principle:** Dependencies flow downward. `Core` owns everything. Nothing below `Core` knows about what's above it.

### 3.2 Core Subsystems

| Subsystem | Responsibility | Key Classes |
|-----------|-----------------|------------|
| **Network/Client** | TCP socket I/O, non-blocking connect, line buffering | `Client`, `ProtocolHandler` |
| **World/Map** | Toroidal grid, tile lookup, resource storage | `Map`, `Tile` |
| **World/Entities** | Players, eggs, food, props with animation | `Player`, `Egg`, `Food`, `Stone`, `Prop` |
| **Graphics/Renderer** | 3D drawing using Raylib, camera control | `Renderer`, `ViewCamera` |
| **Graphics/UI** | Buttons, panels, text, inventory displays | `SubWindowManager`, `SubWindow`, UI components |
| **Core/ActionMap** | Input → action name → behavior indirection | `ActionMap`, `KeyHandler` |

### 3.3 SOLID Principles Applied

**Encapsulation:** Each class has a small public surface exposing only what's needed.

**Interface Segregation:** Pure interfaces (`IEntity`, `ISubWindowComponent`) prevent tight coupling.

**Dependency Injection:** Subsystems are passed in as references; no globals or singletons.

**RAII:** Every GPU resource (`Texture`, `Font`, `Model`) is wrapped in an RAII class that unloads on destruction.

**Example:** `Renderer::drawEntities()` takes a `vector<reference_wrapper<const IEntity>>`. It has no idea if each object is a `Player`, `Egg`, or future entity type—and doesn't need to.

---

## 4. AI Architecture (Python)

### 4.1 Process Model

Each AI process controls exactly one player. Multiple AI processes coordinate through broadcast messages, not shared memory:

```
zappy_ai (process 1)  ←TCP→  NET server  ←TCP→  zappy_ai (process 2)
                                ↓
                           (coordinates via broadcasts)
```

### 4.2 State Machine

Each agent is a state machine with five possible states:

| State | Meaning |
|-------|---------|
| `DEFAULT` | Level ≥ 2; farming resources or waiting for leader election |
| `SURVIVAL` | Food low; abandon current task and seek food |
| `LEADER` | Elected leader; orchestrate team incantation |
| `RALLIED` | Following the leader to an incantation site |
| `ARRIVED` | On the leader's tile; waiting for ritual to begin |

**Level 1 agents** don't use states. They follow a simpler three-phase cycle: collect stone → drop stone → incantate → repeat.

### 4.3 Role-Based Behavior

```
┌─────────────────────────────────────────────┐
│  AI Agent (Algo class)                      │
├─────────────────────────────────────────────┤
│                                             │
│  Level 1:                                   │
│  └─ LevelUp      ← solo collect/drop cycle │
│                                             │
│  Level ≥ 2:                                 │
│  ├─ ElectionManager  ← lowest ID wins       │
│  ├─ LeaderManager    ← farm → call → incant │
│  ├─ RallyManager     ← follow to leader     │
│  ├─ SurvivalManager  ← food seeking         │
│  └─ BroadcastParser  ← team messages       │
│                                             │
└─────────────────────────────────────────────┘
```

### 4.4 Team Coordination via Broadcasts

Broadcasts are prefixed with the team name to filter by team:

```
"Broadcast [team1]Hello_<id>"       ← Announce candidacy for leader
"Broadcast [team1]Leader_<id>"      ← Heartbeat from current leader
"Broadcast [team1]RushLvl8_<id>"    ← Call followers to ritual site
```

Agents ignore broadcasts from other teams, enabling multi-team play on the same server.

### 4.5 Leader Election

1. Level ≥ 2 agent sends `Hello_<id>`
2. All agents track the lowest ID heard
3. After ~100 ticks with no smaller ID, the agent becomes leader
4. Leader commands the team: farm stones → gather followers → incantate

If a smaller ID arrives later, the current leader abdicates and follows the smaller ID.

---

## 5. Data Flow: From Input to Rendering

### 5.1 Server Command Execution

```
Network I/O
    ↓
execute_command("Forward")
    ↓
Command handler (ai/forward.c)
    ├─ Compute new tile (toroidal wrapping)
    ├─ Update ai_t->position
    └─ Schedule command completion
    ↓
GUI receives "ppo <id> <x> <y> <dir>"
    ↓
ProtocolHandler decodes it
    ↓
Core::onPpo() updates Player object
    ↓
Next frame: Renderer draws Player at new position
```

### 5.2 Broadcast Path

```
AI sends: "Broadcast [team1]Hello_<id>"
    ↓
Server queues: { player_fd, "message K, Hello_<id>" }
    ↓
GUI receives broadcast
    ↓
ProtocolHandler::onPbc() routes to Core
    ↓
Core logs to ChatPanel
    ↓
Next frame: ChatPanel renders the message
```

---

## 6. Map Topology: Toroidal Wrapping

The map wraps around on all edges (like Pac-Man):

```
Player at (20, 0) moving NORTH
    ↓
wraps to (20, height-1)
```

This is implemented via doubly-linked tile neighbors. Each tile always has valid north/south/east/west pointers, never `NULL`.

---

## 7. Timing and Synchronization

### 7.1 Server Ticks

The server has a frequency (ticks per second). Each command costs a number of ticks:

| Command | Ticks |
|---------|-------|
| Forward / Turn | 7 |
| Look / Inventory | 1 |
| Broadcast | 7 |
| Take / Set / Fork | 42 |
| Eject | 7 |
| Incantation | 300 |

Food consumption checks happen every tick.

### 7.2 GUI Rendering

The GUI renders at ~60 FPS (independent of server frequency). It interpolates player positions between ticks for smooth animation.

---

## 8. Connection Protocol

### 8.1 AI Handshake

```
Server → AI:     "WELCOME <server_message>"
AI → Server:     "<team_name>"
Server → AI:     "<available_slots>"
Server → AI:     "<map_width> <map_height>"
AI → Server:     [commands] (Forward, Look, etc.)
```

### 8.2 GUI Handshake

```
Server → GUI:    "WELCOME <server_message>"
GUI → Server:    "GRAPHIC"
Server → GUI:    [events] (pnw, ppo, bct, etc.)
```

The GUI can query anytime (msz, bct, mct, ppo, plv, pin) and receives push events for all game updates.

---

## 9. Why This Design?

| Design Choice | Benefit |
|---------------|---------|
| **Event-driven server** | Predictable timing, no race conditions |
| **Single-threaded** | Easier debugging, deterministic replays |
| **Text protocol** | Easy to implement clients in any language |
| **Layered GUI** | Components can be tested/reused independently |
| **RAII for GPU resources** | No resource leaks, exception-safe |
| **AI state machine** | Coordination logic is centralized and testable |
| **Broadcast team filtering** | Multiple teams can coexist on one server |

---

## 10. Scalability Considerations

- **Map size**: Limited by memory (2D grid); tested up to 100×100
- **Players**: Limited by available connections (~1000 per server)
- **Network**: Plaintext protocol is compact; ~50 bytes per command average
- **CPU**: Server is CPU-light (mostly I/O-bound on LAN); no bottleneck observed with 100+ agents

---

**For more details, see component-specific documentation:**
- [GUI Internals](../GUI/Docs/02-Architecture.md)
- [AI Strategy](../AI/Docs/Zappy_AI.md)
- [Protocol Reference](./API_REFERENCE.md)
