# Zappy

> A full-stack distributed multiplayer simulation platform for teaching concurrent systems, network protocols, and collaborative AI strategies.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](./LICENSE)
![Language](https://img.shields.io/badge/Languages-C%20%7C%20C%2B%2B%20%7C%20Python-blue)
![Build](https://img.shields.io/badge/Build-CMake%20%7C%20Make-green)
![Platform](https://img.shields.io/badge/Platform-Linux-orange)

---

## 🎮 What is Zappy?

Zappy is a **complete distributed system** where multiple AI-controlled players (trantorian creatures) compete in teams to gather resources, level up through collaborative rituals, and reach maximum level to win. 

- **One authoritative server** (C, event-driven) manages game state and rules
- **Multiple AI clients** (Python) connected over TCP, each controlling one player
- **One spectator GUI** (C++/Raylib) renders the world in real-time 3D

All components communicate exclusively through **well-defined text protocols**, making it ideal for:
- Teaching network programming and distributed systems
- Practicing AI algorithm design (leader election, pathfinding, coordination)
- Understanding event-driven server architecture
- Implementing collaborative team strategies

---

## ✨ Key Features

| Feature | Details |
|---------|---------|
| 🌍 **Shared World** | Toroidal map with food, minerals, and spawning eggs |
| 🤖 **AI Agents** | Python-based intelligent agents with perception, movement, and communication |
| 📊 **Real-Time Visualization** | 3D GUI showing all players, resources, and broadcasts live |
| 🏆 **Team Gameplay** | Multiple teams competing; win by unanimous team incantation at max level |
| 📡 **Clean Protocol** | Text-based TCP protocol: easy to implement clients in any language |
| ⚙️ **Server Logic** | Event-driven polling server; no threads, no race conditions |
| 🧠 **Advanced AI** | Automatic leader election, resource coordination, pathfinding |
| 🎵 **Rich UI** | Tile inspection, player tracking, chat/broadcast history, settings, animations |

---

## 🛠 Tech Stack

| Layer | Technologies |
|-------|--------------|
| **Game Server** | C, POSIX sockets, `poll(2)` event loop, no external deps |
| **GUI Client** | C++17, Raylib, CMake, RAII patterns, component architecture |
| **AI Agents** | Python 3, stdlib only (no external dependencies) |
| **Build** | Make, CMake |
| **Testing** | Custom C test framework |
| **Documentation** | Markdown, Doxygen-ready |

---

## 📁 Project Structure

```
Zappy/
├── NET/           Game server (C) - authoritative game state & networking
├── GUI/           Spectator client (C++/Raylib) - 3D visualization
├── AI/            Player client (Python) - intelligent agent
├── docs/          Technical documentation
└── Makefile       Root build orchestrator
```

Each component runs independently and communicates via TCP on a shared port.

For detailed architecture, see [docs/ARCHITECTURE.md](./docs/ARCHITECTURE.md).

---

## 🚀 Quick Start

### Prerequisites

- **Linux** with a graphical session (X11 or Wayland)
- **Build tools**: `make`, `cmake ≥ 3.10`, C/C++ compiler
- **Raylib** (graphics library) - install via:
  ```bash
  # Ubuntu/Debian
  sudo apt-get install libraylib-dev

  # Or build from source (recommended)
  git clone https://github.com/raysan5/raylib.git
  cd raylib && mkdir build && cd build
  cmake .. -DBUILD_SHARED_LIBS=OFF && make && sudo make install
  ```
- **Python 3.7+** (for AI clients)

### Build Everything

```bash
cd Zappy
make all
```

This produces three executable artifacts:
- `zappy_server` - The game server
- `zappy_gui` - The graphical client
- `zappy_ai` - The AI player script

### Run a Game

**Terminal 1 — Start the server:**
```bash
./zappy_server -p 4242 -x 30 -y 30 -n team1 team2 -c 4 -f 100
```

**Terminal 2 — Start the GUI:**
```bash
./zappy_gui -p 4242 -h localhost
```

**Terminal 3+ — Start AI players (one per terminal):**
```bash
./zappy_ai -p 4242 -n team1 -h localhost
./zappy_ai -p 4242 -n team2 -h localhost
# ... repeat for more players
```

Watch the simulation unfold in the GUI! Players will move, gather resources, and attempt incantations.

For detailed setup and troubleshooting, see [docs/GETTING_STARTED.md](./docs/GETTING_STARTED.md).

---

## 📚 Documentation

| Document | Purpose |
|----------|---------|
| [docs/GETTING_STARTED.md](./docs/GETTING_STARTED.md) | Installation, build, and first-run guide |
| [docs/ARCHITECTURE.md](./docs/ARCHITECTURE.md) | System design, data flows, component responsibilities |
| [docs/API_REFERENCE.md](./docs/API_REFERENCE.md) | Complete protocol specification (AI, GUI, server commands) |
| [docs/DEPLOYMENT.md](./docs/DEPLOYMENT.md) | Production setup, environment configuration, troubleshooting |
| [CHANGELOG.md](./CHANGELOG.md) | Version history and release notes |
| [CONTRIBUTORS.md](./CONTRIBUTORS.md) | Team credits and acknowledgments |

**Component-specific docs:**
- [GUI Architecture](./GUI/Docs/02-Architecture.md) - Internal GUI design patterns
- [GUI C++ Quality](./GUI/Docs/03-Cpp-Design-And-Extensibility.md) - Best practices applied
- [AI Strategy](./AI/Docs/Zappy_AI.md) - AI algorithm details and state machine

---

## 📖 Game Rules & Mechanics

### Objective
Players spawn in teams. Each team must coordinate to:
1. Gather scattered resources (food, minerals)
2. Level up individually through resource collection and ritual
3. Reach maximum level (8) through a team incantation
4. Be the first team with all players at level 8 to win

### Core Mechanics
- **Perception**: Each player can see 8 tiles in their facing direction
- **Movement**: Forward, left turn, right turn, each costs time
- **Inventory**: Carry up to 10 resource units
- **Incantation**: Multi-player ritual requiring specific resources and coordination
- **Broadcasting**: Send team-wide messages with 1-hop radius
- **Forking**: Create new players via eggs on the map

---

## 🏗 Architecture Highlights

**Server (NET)**
- Event-driven polling loop: ~4,284 lines of C
- 12+ AI commands, 18+ GUI query commands
- No threads; deterministic and predictable
- Tile-based world with toroidal wrapping

**GUI (C++)**
- Real-time 3D rendering using Raylib
- Component-based UI system
- ~8,196 lines following SOLID principles
- Extensible animation & model caching

**AI (Python)**
- State machine with roles: level-up, leader, follower, survivor
- Automatic leader election by lowest ID
- Intelligent resource gathering & pathfinding
- Team coordination via broadcast protocol

For in-depth design, see [docs/ARCHITECTURE.md](./docs/ARCHITECTURE.md).

---

## 📋 Project Stats

- **Total Code**: ~13,976 lines
- **Server Tests**: 6 comprehensive test suites
- **Assets**: 97 files (3D models, textures, fonts, audio)
- **Animations**: 10 character sequences, 8 level variants
- **Build System**: Unified Makefile + CMake for modular builds

---

## 🐛 Known Limitations

- GUI requires X11/Wayland display; SSH X-forwarding may be slow
- AI clients are compute-lite; optimized for clarity over speed
- Map size limited by memory (tested up to 100x100)
- Network protocol is plaintext (fine for LAN, use VPN for untrusted networks)

---

## 📞 Contact & Credits

**Project Context**
- Epitech School Educational Project (2024-2026)
- Semester 4: Systems & Networking module

**Get Involved**
- Found a bug? [Open an issue](../../issues)
- Want to contribute? See [docs/CONTRIBUTING.md](./docs/CONTRIBUTING.md)
- Have questions? Check existing documentation or open a discussion

**Attriredits & Attribution

**Project Context**
- Epitech School Educational Project (2024-2026)
- Semester 4: Systems & Networking module

**Development Team**
- See [CONTRIBUTORS.md](./CONTRIBUTORS.md) for detailed credits by component
---

# Additional Technical Documentation

> The following sections provide deep technical reference. For recruiter-friendly overview, see the sections above.

---

# 1. Architecture overview

## 1.1 Subsystems

Zappy is composed of three independent processes that communicate exclusively over TCP.

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

- **NET** is the authoritative game server. It owns the world state, schedules commands, and manages all TCP connections.
- **GUI** connects as a special client (type `GUI`). It receives push notifications for all game events and can query world state via dedicated commands.
- **AI** clients connect as players (type `AI`). Each AI process controls one player and sends text commands one at a time.

The server is the single source of truth. The GUI and AI never communicate with each other directly.

## 1.2 Server internal layers

```
main()
  └── init_server()          ← allocates server_t + network_t + world_t
  └── run_server()           ← main loop
        └── poll()           ← blocks up to next_command_time ms
        └── handle_tick()
              └── refresh_ais()        ← food consumption / death
              └── refresh_commands()   ← execute due commands
              └── win()                ← check win condition
        └── handle_poll_events()
              └── new_connection()     ← accept() + WELCOME
              └── get_split_commands() ← recv + partial buffering
              └── execute_commands()   ← dispatch AI/GUI/UNKNOWN
```

## 1.3 Core data structures and their relationships

```
server_t
├── args_t          ← CLI arguments (port, width, height, freq, teams…)
├── network_t       ← poll_fds[], connections linked list, blacklist
└── world_t
    ├── args_t *    ← shared reference
    ├── tile_t *grid          ← flat array [height × width], doubly linked as torus
    ├── ai_t *ais             ← global linked list of all live AI players
    ├── gui_t *gui            ← linked list of connected GUI clients
    ├── team_t *teams         ← linked list of teams
    │     └── ai_t **players  ← fixed-size array (eggs_per_team slots)
    ├── command_t *command_queue   ← global queue sorted by remaining_time
    ├── size_t resource_counts[7]  ← global inventory for idempotent respawn
    └── connection_t **connections ← pointer into network_t's list (shared)
```

### Key struct snapshots

**`tile_t`** — one cell of the map, linked to its 4 neighbours:
```c
typedef struct tile {
    int x, y;
    struct tile *north, *south, *east, *west;
    tile_content_t *content;   // resources[7] + player_t *players
} tile_t;
```

**`ai_t`** — a live player or an unhatched egg (`level == 0`):
```c
typedef struct ai {
    size_t id;
    size_t level;          // 0 = egg, 1–7 = playing, 8 = max
    team_t *team;
    direction_t direction; // NORTH / EAST / SOUTH / WEST
    int inventory[7];
    tile_t *position;
    command_t *local_commands;  // per-player FIFO queue
    egg_type_t egg_type;        // INITIAL or FORK
    time_t last_check;          // for food depletion timing
    bool is_incanting;
    tile_t *incant_tile;
    struct ai *next;
} ai_t;
```

**`connection_t`** — one TCP client:
```c
typedef struct connection_s {
    int fd;
    client_type_t type;          // GUI / AI / UNKNOWN
    client_data_t cli;           // union { ai_t *ai; gui_t *gui; }
    char *partial_command;       // TCP partial read buffer
    size_t partial_len;
    struct connection_s *next;
} connection_t;
```

**`command_t`** — one scheduled action:
```c
typedef struct command_s {
    char **arguments;            // NULL-terminated, heap-allocated
    ai_t *player;
    int player_fd;
    time_t remaining_time;       // milliseconds until execution
    void (*handler)(world_t *, struct command_s *);
    struct command_s *next;
} command_t;
```

## 1.4 Dual command-queue model

Every AI command goes through two queues:

```
Client sends "Forward\n"
       │
       ▼
execute_commands()
       │
       ├─ player already in global queue?
       │       YES → append to ai->local_commands (FIFO)
       │       NO  → add_global_command() → insert into world->command_queue
       │                                    sorted by remaining_time
       ▼
refresh_commands()  (called every tick)
       │
       ├─ for each due command in command_queue:
       │       execute_command()
       │       remove from global queue
       │       pop next from local_commands → add to global queue (deferred)
       └─ update world->next_command_time = head->remaining_time
```

This enforces that each player has at most one command executing in the global queue at a time, while allowing the AI to pipeline up to 10 commands in its local queue.

## 1.5 Server main loop timing

```
poll(poll_fds, nfds, next_command_time)
      │
      ├── timeout (ret == 0): no I/O but a tick fired → skip handle_poll_events
      ├── ret > 0: I/O + tick → handle both
      └── ret < 0: signal (EINTR) or error → skip everything, continue loop
```

`next_command_time` is the `remaining_time` of the head of `command_queue` in milliseconds. When the queue is empty, `poll` is called with `-1` (infinite timeout).

---

# 2. Protocol & command reference

## 2.1 Connection handshake

```
Server → Client   "WELCOME\n"
Client → Server   "<team_name>\n"       (or "GRAPHIC\n" for GUI)

Server → AI       "<free_slots>\n"
                  "<map_width> <map_height>\n"

Server → GUI      (no additional handshake; GUI can query any time)
```

If the team is full the server closes the connection. If the server is full it sends `"Server is full\n"` then closes. Banned IPs (firewall mode) receive `"You're banned from this server\n"`.

## 2.2 AI command reference

All times are expressed in **ticks**. One tick = `1000 / freq` ms.

| Command | Syntax | Cost (ticks) | Success response | Failure response |
|---|---|---|---|---|
| `Forward` | `Forward` | 7 | `ok` | — |
| `Right` | `Right` | 7 | `ok` | — |
| `Left` | `Left` | 7 | `ok` | — |
| `Look` | `Look` | 7 | `[tile0, tile1, …]` | — |
| `Inventory` | `Inventory` | 1 | `[food N, linemate N, …]` | — |
| `Broadcast` | `Broadcast <text>` | 7 | `ok` | — |
| `Connect_nbr` | `Connect_nbr` | 0 | `<N>` (free slots) | — |
| `Fork` | `Fork` | 42 | `ok` | — |
| `Eject` | `Eject` | 7 | `ok` | `ko` |
| `Take` | `Take <resource>` | 7 | `ok` | `ko` |
| `Set` | `Set <resource>` | 7 | `ok` | `ko` |
| `Incantation` | `Incantation` | 300 | `Current level: N` | `ko` |

### Notes

- **Pipelining**: the server accepts up to 10 queued commands per AI. Beyond that, incoming commands are silently dropped.
- **Incantation**: the server responds `"Elevation underway"` immediately when the incantation starts, then `"Current level: N"` when it completes (300 ticks later). Other players on the same tile at the same level are elevated automatically.
- **Look** response format: tiles are comma-separated, resources within a tile are space-separated. Tile 0 is the player's current tile. Tiles are ordered left-to-right, front-to-back, expanding one row per vision level: `[t0, t1 t2 t3, t4 t5 t6 t7 t8, …]`.
- **Eject** removes all other players and destroys all fork eggs on the current tile. Ejected players land on the tile in front of the ejector and receive `"eject: K"` where K is the direction (1–8, 0 = same tile).
- **Broadcast** sends `"message K, <text>"` to every AI. K is the direction the sound comes from (0 = same tile, 1–8 = cardinal/diagonal).
- **Dead**: if an AI has no food left, the server sends `"dead\n"` and closes the connection. This can happen at any point, even mid-command.

### Resources accepted by `Take` / `Set`

`food`, `linemate`, `deraumere`, `sibur`, `mendiane`, `phiras`, `thystame`

## 2.3 GUI command reference

### GUI → Server (queries)

| Command | Arguments | Response |
|---|---|---|
| `msz` | — | `msz <W> <H>` |
| `bct` | `<X> <Y>` | `bct <X> <Y> <food> <linemate> <deraumere> <sibur> <mendiane> <phiras> <thystame>` |
| `mct` | — | one `bct` line per tile |
| `tna` | — | one `tna <name>` line per team |
| `ppo` | `<id>` | `ppo <id> <X> <Y> <dir>` |
| `plv` | `<id>` | `plv <id> <level>` |
| `pin` | `<id>` | `pin <id> <X> <Y> <food> <linemate> …` |
| `sgt` | — | `sgt <freq>` |
| `sst` | `<freq>` | `sst <freq>` (also sets the server frequency) |

Direction encoding in `ppo`: `1=N 2=E 3=S 4=W`.

### Server → GUI (push events)

| Event | Format | Trigger |
|---|---|---|
| `pnw` | `pnw <id> <X> <Y> <dir> <lvl> <team>` | new player connects |
| `ppo` | `ppo <id> <X> <Y> <dir>` | player moves |
| `plv` | `plv <id> <lvl>` | player levels up |
| `pin` | `pin <id> <X> <Y> <food> …` | player inventory changes |
| `pex` | `pex <id>` | player ejects |
| `pbc` | `pbc <id> <text>` | player broadcasts |
| `pic` | `pic <X> <Y> <lvl> <id> …` | incantation starts |
| `pie` | `pie <X> <Y> <result>` | incantation ends (1=ok, 0=ko) |
| `pfk` | `pfk <id>` | player forks |
| `enw` | `enw <egg_id> <parent_id> <X> <Y>` | egg laid |
| `ebo` | `ebo <egg_id>` | egg hatches |
| `edi` | `edi <egg_id>` | egg destroyed (by eject) |
| `pdi` | `pdi <id>` | player dies |
| `pdr` | `pdr <id> <resource>` | player drops resource |
| `pgt` | `pgt <id> <resource>` | player picks up resource |
| `seg` | `seg <team>` | a team wins |

## 2.4 Broadcast direction algorithm

The server computes the direction number K from 0–8 as follows:

1. Compute the torus shortest vector from broadcaster to receiver: pick the shorter of the two paths on each axis (wrapping considered).
2. Map the vector to an absolute 8-direction (`ABS_NORTH`, `ABS_SOUTH_EAST`, etc.) using a 3×3 lookup table on the sign of each component.
3. Rotate the absolute direction by the receiver's facing direction using modular arithmetic on a clock with 8 positions (0=North, clockwise).
4. The result is the direction the receiver must move toward to find the broadcaster.

K=0 means broadcaster and receiver are on the same tile.

---

# 3. AI strategy guide

## 3.1 State machine overview

The Python AI runs a single `Algo` instance per process. On each tick it calls
`next_move()`, which reads the current `State` and returns the next command
string to send. The agent identifies itself with a random ID (`self.id`,
drawn at startup) and a `team_name` passed on the command line.

```
State           | Meaning
----------------|----------------------------------------------------
DEFAULT         | Level >= 2 default behaviour: gather, run election
SURVIVAL        | Food is low, drop everything and go eat
LEADER          | Coordinates the team's incantation ritual
ARRIVED         | Standing on the leader's tile after a rally
RALLIED         | Walking toward the leader in response to a call
```

A level-1 agent does not use any of the states above. It follows a separate
three-phase cycle driven by `LevelUp.phase` inside `rush_level_one()`:
`collect` (gather one `linemate`) → `drop` (`Set` it on the tile) → `incant`
(`Incantation`), looping back to `collect` after each level gained.

```
 Level 1 agent                       Level 2+ agent
+------------------+                +----------------------------+
| phase: collect   |                | _next_move_level_2_plus()  |
| gather linemate  |                | sends Hello_<id>           |
+--------+---------+                +-------------+--------------+
         v                                        |
+-------------------+             +---------------+----------------+
| phase: drop       |             |                                |
| Set linemate      |       becomes LEADER                   stays follower
+--------+----------+      (lowest id wins)              (SURVIVAL / DEFAULT
         v                        |                         farming loop)
+--------------------+            v                                |
| phase: incant      |        LeaderManager.next_move()            |
| Incantation        |       farm stones -> lock food -> call -----+
+--------------------+       RushLvl8_ -> wait for players ->
                              stabilize -> Incantation
```

### State descriptions

| State | Description | Set by |
|---|---|---|
| `DEFAULT` | Level >= 2 default behaviour; gathers and farms until an election round starts | initial state once level reaches 2 |
| `SURVIVAL` | Drops whatever it's doing and pathfinds to the nearest `food` tile | `SurvivalManager.update_state()` once `survival_time` < `SURVIVAL_LOW_THRESHOLD` (3000); cleared once it climbs back above `SURVIVAL_RECOVERED_THRESHOLD` (3700) |
| `LEADER` | Elected agent; runs the farm -> call -> stabilize -> incantation cycle for the team | `ElectionManager.try_become_leader()` |
| `ARRIVED` | Reached direction 0 on a `RushLvl8_` broadcast (standing on the leader's tile) | `RallyManager.on_rush_broadcast()` when `direction == 0` |
| `RALLIED` | Has heard `RushLvl8_<leader_id>` and is walking toward the source direction | `RallyManager.on_rush_broadcast()` when `direction != 0` |

`SurvivalManager.update_state()` never overrides `ARRIVED`, `RALLIED`, or
`LEADER` — those three are exclusively driven by the broadcast/election/leader
managers.

## 3.2 Team identification and broadcasts

Every broadcast the AI sends is wrapped by a single helper:

```python
def make_broadcast(self, text):
    return f"Broadcast [{self.team_name}]{text}\n"
```

All broadcast types go through it — `Hello_<id>` (election), `Leader_<id>`
(heartbeat), and `RushLvl8_<id>` (call to the incantation tile). On the
receiving side, `BroadcastParser.parse_broadcast()` discards any message
whose text doesn't start with `[<our_team_name>]`:

```python
team_prefix = f"[{self.agent.team_name}]"
if not text.startswith(team_prefix):
    return
```

This is what lets several teams' AIs coexist on the same map: a message
broadcast by an agent of another team never reaches the election, leader, or
rally logic of this agent — it is dropped before any of the `Hello_`,
`Leader_`, or `RushLvl8_` handlers run.

`RushLvl8_` is parsed regardless of level, as long as the receiving agent is
level >= 2 and either low on survival margin or already `RALLIED`. `Hello_`
and `Leader_` are only parsed once `agent.level >= 2`.

## 3.3 Role election mechanism

Election is handled by `ElectionManager` (`algo/election.py`) and works as
follows:

1. A level >= 2 agent that hasn't yet announced itself sends `Hello_<id>`.
2. Every other agent's `BroadcastParser` routes `Hello_` and `Leader_`
   messages into `ElectionManager.on_hello_broadcast` /
   `on_leader_broadcast`, which track the lowest ID heard so far
   (`agent.lowest_id_heard`).
3. After `ELECTION_SETTLE_TURNS` (100 ticks) without hearing a smaller ID,
   `try_become_leader()` promotes the agent: `is_leader = True`,
   `leader_id = self.id`, and it immediately checks whether the team needs
   more players (`_check_team_size_and_fork`, see 3.4).
4. If a smaller ID is heard later — even after becoming leader — the agent
   abdicates via `_abdicate_if_needed`: it clears `is_leader`,
   `ready_for_ritual`, `farming_food`, `leader_stopped`, and its
   `pending_moves`, and falls back to following the smaller-ID agent.

Key fields on `Algo` involved in election: `self.id`, `self.is_leader`,
`self.leader_confirmed`, `self.target_leader_id`, `self.lowest_id_heard`,
`self.election_wait`, `self.election_done`.

## 3.4 Auto-forking new players

Right after an agent confirms its leadership, `_check_team_size_and_fork`
checks how many distinct agent IDs have been seen so far (via `Hello_` /
`Leader_` broadcasts, plus itself) against `TARGET_TEAM_SIZE` (8), and queues
enough `Fork` commands to bring the team up to that size:

```python
missing = TARGET_TEAM_SIZE - total_seen
for _ in range(missing):
    agent.pending_moves.append(agent.make_broadcast(f"Leader_{agent.id}"))
    agent.pending_moves.append("Fork\n")
```

This runs once per leader (guarded by `fork_done`). Separately, the
`Bonus.py` entry point checks the free-slot count returned by the server on
connect and spawns a sibling Python process via `subprocess.Popen` whenever
slots are already open, so that one OS process exists per available egg
slot.

## 3.5 Leader behaviour — farm, call, stabilize, incant

`LeaderManager` (`algo/leader.py`) drives the leader through two phases:

**Farm phase** (`ready_for_ritual == False`):
1. While any `LEVEL_UP_LEADER` stone requirement isn't met yet, pathfind to
   and `Take` the next missing stone (`farm_target`).
2. Once all stones are held, lock in a one-time food decision: if
   `survival_time >= FOOD_SAFE_THRESHOLD` (4300) the leader stops farming
   food entirely (`farming_food = True`); otherwise it keeps farming food
   until that threshold is reached.
3. When both conditions hold, `ready_for_ritual` is set and the leader does
   not move from that tile again.

**Ritual phase** (`ready_for_ritual == True`):
1. First call: broadcast `RushLvl8_<id>` to summon the team and mark
   `leader_stopped = True`.
2. Every tick, count `player`s on its own tile via `Look`
   (`players_on_my_tile`). If below `MIN_PLAYERS_LEVEL_UP_3` (6), it
   re-broadcasts `RushLvl8_<id>` every `LEADER_RECALL_INTERVAL` (2) ticks
   while waiting.
3. Once 6+ players are confirmed present, it waits
   `STABILIZATION_TURNS_REQUIRED` (5) extra ticks (+3 more on the very first
   ritual, via `FIRST_RITUAL_EXTRA_WAIT`) to let stragglers settle, then
   starts the incantation: clears the tile of non-food items, `Set`s every
   stone `LEVEL_UP_REQUIREMENTS[level]` calls for, and issues `Incantation`.
4. A `LEADER_HEARTBEAT_INTERVAL` (50 ticks) `Leader_<id>` broadcast is sent
   independently of the phase above, so followers can re-sync onto the
   current leader at any time.

Followers receiving `RushLvl8_<leader_id>` go through `RallyManager` (3.6)
instead of running their own farming logic while rallied.

## 3.6 Rallying to the leader

`RallyManager` (`algo/rally.py`) reacts to `_handle_rush_lvl8` in
`broadcast.py`: a follower locks onto the first `RushLvl8_` leader ID it
hears (or the one it's already targeting), and
`agent.rally.on_rush_broadcast(direction)` sets its state:

- `direction == 0` -> already on the leader's tile -> `State.ARRIVED`, next
  move sends `Inventory`.
- `direction != 0` -> `State.RALLIED`; `last_rally_direction` is stored and
  consumed on the next tick via a fixed direction-to-move table
  (`DIRECTION_TO_MOVE`): directions 1/2/8 -> `Forward`, 3/4 -> `Left`,
  5/6/7 -> `Right`. After moving once, the agent waits for the next
  broadcast to get a fresh direction.

A follower whose `survival_time` drops to `RALLY_ABORT_SURVIVAL_TIME` (0)
while rallied abandons the rally and falls back to `State.DEFAULT` /
survival farming.

## 3.7 Level-up resource requirements

Used by the leader (`LEVEL_UP_REQUIREMENTS`, for the `Set` plan before
`Incantation`) and by the level-1 solo path (same dict, level-1 entry only):

```
Level ->  | linemate | deraumere | sibur | mendiane | phiras | thystame
----------|----------|-----------|-------|----------|--------|----------
1 -> 2    | 1        | -         | -     | -        | -      | -
2 -> 3    | 1        | 1         | 1     | -        | -      | -
3 -> 4    | 2        | -         | 1     | -        | 2      | -
4 -> 5    | 1        | 1         | 2     | -        | 1      | -
5 -> 6    | 1        | 2         | 1     | 3        | -      | -
6 -> 7    | 1        | 2         | 3     | -        | 1      | -
7 -> 8    | 2        | 2         | 2     | 2        | 2      | 1
```

`MIN_PLAYERS_LEVEL_UP_3 = 6` is the flat player-count threshold the leader
waits for before incanting at every level >= 3 (the code waits for the same
6-player headcount on the tile at every level past level 2, regardless of
which level is being attempted).

The leader additionally carries a much larger personal stockpile before it
even enters the ritual phase — `LEVEL_UP_LEADER` (linemate 8, deraumere 8,
sibur 10, mendiane 5, phiras 6, thystame 1) — so that it can supply every
incantation from level 2 up to level 8 without needing to re-farm mid-ritual.

## 3.8 Inventory tracking

`Inventory` (`algo/inventory.py`) keeps a `data` dict seeded from
`INVENTORY_TEMPLATE` (all 7 resources at 0). It is repopulated wholesale
every time an `Inventory` server response is parsed, and nudged
incrementally by `update_from_command` right after a successful `Take`:
taking `food` also adds `FOOD_DURATION` (126) ticks to `survival_time`;
taking any other resource increments its counter directly.

Stone-requirement checks live in two places: `LevelUp` (level-1 solo path,
`check_need_level_up` / `get_target_object`) and inline on `LeaderManager`
(`has_all_leader_items`, `get_missing_leader_items`,
`get_missing_leader_stone`) for the team path.

## 3.9 Look vision parsing

`Look.parse_look_response` (`commande/look/look.py`) turns the server's flat
`[t0, t1 t2 t3, t4 t5 t6 t7 t8, ...]` reply into `vision[row][col]`, where
row 0 is always the agent's own tile and row *i* has `2i + 1` tiles,
matching the protocol exactly. The number of rows built is `level + 1` (a
level-1 agent parses 2 rows / 4 tiles total, a level-7 agent parses 8 rows /
64 tiles).

## 3.10 Pathfinding

`Look.pathfinding_move` first tries `_move_to_visible_target`: it scans the
already-parsed vision for the nearest tile containing the target object
(closest by `row + |col - row|`), and emits the `Forward` / `Left` / `Right`
sequence to reach it, followed by `Take <object>` (with an extra `Look`
inserted first when the target is `food`, to refresh vision right before
picking it up). If nothing matching is visible, it falls back to
`_sweep_move`, a deterministic sweep across the map: walk `vision_range`
tiles forward, `Look`, repeat, and at each map edge turn `Right` / `Left`
alternately to cover the next row.

Broadcast-direction following uses a separate, simpler table
(`DIRECTION_TO_MOVE` in `rally.py`, see 3.6) rather than this pathfinder —
rallying only needs one move per received direction, not a multi-step plan.

## 3.11 Survival mechanics

- One `food` unit equals `FOOD_DURATION` = 126 ticks of life, tracked as a
  budget in `survival_time` (not as a raw inventory count).
- `survival_time` enters `State.SURVIVAL` below `SURVIVAL_LOW_THRESHOLD`
  (3000 ticks) and exits back to `DEFAULT` above
  `SURVIVAL_RECOVERED_THRESHOLD` (3700 ticks); the gap between the two
  thresholds prevents the state from flapping at the boundary.
- While in `SURVIVAL`, the agent pathfinds straight to the nearest visible
  `food` via `SurvivalManager.next_move()` (same pathfinder as 3.10). A
  level-2 agent additionally stops topping up food once `survival_time`
  passes `LEVEL_2_FOOD_CAP` (4600), to avoid over-farming food at the
  expense of stones before the team's first incantation.

---

# 4. Developer onboarding guide

## 4.1 Prerequisites

| Tool | Purpose |
|---|---|
| `gcc` / `epiclang` | Compile the C server (`NET/`) |
| `cmake` ≥ 3.16 | Build the C++ GUI |
| `raylib` | GUI rendering library |
| Python ≥ 3.10 | AI client |
| `criterion` | C unit tests |
| `gcov` / `lcov` | Coverage reporting |

## 4.2 Build instructions

```bash
# Build everything
make all

# Server only
make zappy_server          # runs make -C NET

# GUI only
make zappy_gui             # cmake -B build -S GUI && cmake --build build

# AI only
make zappy_ai              # cp main.py + chmod +x

make zappy_ai_bonus        # cp Bonus.py + chmod +x

# Tests
make tests_run             # runs make -C NET/tests tests_run

# Coverage (HTML report in NET/tests/coverage/)
make coverage              # runs make -C NET/tests coverage

# Clean
make fclean
```

The server binary is `zappy_server`. The GUI binary is placed in `build/`.

### Running the server

```bash
./zappy_server -p <port> -x <width> -y <height> -n <team1> [<team2>...] \
               -c <eggs_per_team> -t <freq> [-firewall] [-v]
```

Example:
```bash
./zappy_server -p 4242 -x 20 -y 20 -n red blue -c 5 -t 10
```

### Running the AI

```bash
./zappy_ai -h <machine_ip> -p <port> -n <team_name>
```

Example:
```bash
./zappy_ai -h 127.0.0.1 -p 1234 -n red
```

## 4.3 Codebase tour — `NET/`

```
NET/
├── Include/               ← all .h headers (mirrored structure)
│   ├── Execution/         ← command.h, ai_commands.h, gui_commands.h, incantation.h
│   ├── Logger/            ← logger.h
│   ├── Parser/            ← cli_opt.h, parser.h
│   ├── Server/            ← args.h, blacklist.h, connection.h, network.h, server.h
│   ├── Utils/             ← utils.h
│   └── World/             ← ai.h, gui.h, map.h, resources.h, team.h, world.h
└── Src/
    ├── Execution/
    │   ├── ai/            ← one file per AI command (forward.c, look.c, …)
    │   ├── command_handling/  ← add_command, create_command, refresh_commands, …
    │   ├── gui/
    │   │   ├── input/     ← bct.c, mct.c, msz.c, pin.c, plv.c, ppo.c, sgt.c, sst.c, tna.c
    │   │   └── output/    ← ebo.c, edi.c, enw.c, pbc.c, pdi.c, pdr.c, pex.c, pfk.c,
    │   │                     pgt.c, pic.c, pie.c, pnw.c
    │   └── utils/         ← move_from_directions.c (get_front/left/right)
    ├── Logger/            ← display_config, display_help, display_usage,
    │                        logger_client, logger_server, logger_incantation
    ├── Parser/            ← parse_args, parse_teams, get_split_commands, …
    ├── Server/
    │   ├── Args/          ← init_destroy.c
    │   ├── Network/
    │   │   ├── Blacklist/ ← init_destroy.c, is_banned.c
    │   │   └── Connection/← init_destroy.c, get_connection.c
    │   ├── disconnect_connection.c
    │   ├── get_request.c
    │   ├── init_destroy.c
    │   ├── new_connection.c
    │   └── run_server.c
    ├── Utils/             ← free_array, get_fd_from_connections, int_to_str, send_to_guis
    ├── World/
    │   ├── Ais/           ← init, destroy, bloom_egg, refresh_ais, win, is_available_team
    │   ├── Guis/          ← init_destroy
    │   ├── Map/           ← init_destroy, get_tiles, get_torus_shortest_path,
    │   │                    interpret_sound_direction
    │   ├── Resources/     ← spawn_resources
    │   ├── Teams/         ← init_destroy
    │   └── init_destroy.c
    └── main.c
```

## 4.4 Epitech norm — enforced rules

Every `.c` and `.h` file in the project must comply. Violations cause a grade penalty.

| Rule | Constraint |
|---|---|
| Function length | ≤ 20 lines (excluding braces and blank lines) |
| Function parameters | ≤ 4 parameters |
| Branches per function | ≤ 3 (if / else if / else / switch each count) |
| Functions per file | ≤ 5 |
| Line width | ≤ 80 columns |
| Variable declarations | C89-style: all at top of block, before any statement |
| No `else` after `return` | `if (x) return y;` then fall-through, no trailing `else` |
| File-local functions | must be declared `static` |
| Naming convention | `snake_case` for all identifiers |
| No global mutable state | except `volatile bool running` in `run_server.c` |

## 4.5 Known pitfalls and past bugs

### Memory — `create_command` / `add_global_command`

`create_command` heap-allocates the `command_t` and duplicates the `arguments` array. If `add_global_command` rejects the command (e.g. incantation precondition fails), it must `free_array(arguments)` and `free(command)` — not just `return`. Missing either produces a leak Valgrind traces back to `create_command`.

### List mutation while iterating — `refresh_ais`

`refresh_ais` iterates `world->ais`. When `handle_dead_ai` calls `destroy_ai`, it removes the node from the list. Always capture `next = current->next` before calling any function that may free the current node:

```c
while (current) {
    next = current->next;   // capture BEFORE processing
    process_ai(world, current, connections, curr);
    current = next;
}
```

### Infinite death loop — `handle_dead_ai`

The bug was iterating `world->ais` looking for a matching AI and operating on the head of the list instead of the matched node. Fix: walk `connection_t *conns` (not `world->ais`) and break on `cnx->cli.ai == ai`.

### Shallow connection pointer copy

`server->world->connections = server->network->connections` makes `world->connections` point to the list head value at assignment time. When `new_connection` or `disconnect_connection` changes the head (prepend/remove), `world->connections` becomes stale. The correct pattern is to store `connection_t **` (pointer to pointer) so dereferencing always yields the current head.

### `poll()` `EINVAL` from oversized `nfds`

`nfds` must not exceed `RLIMIT_NOFILE`. Passing `max_connections + 1 > RLIMIT_NOFILE` makes `poll` return `-1` with `EINVAL`. The fix is to cap `max_connections` at `get_max_connections() - 1` (where `get_max_connections` calls `getrlimit(RLIMIT_NOFILE, …)`).

### Duplicate AI ID in `spawn_team_eggs`

If the ID counter was not correctly incremented between eggs during initial spawn, multiple AIs shared the same ID. GID assignment must happen inside the per-egg loop, not once before it.

### Command local queue — pointer aliasing on insert

When promoting a command from `local_commands` to the global queue, the node must be detached from the local list first (`pop_local_command`), then inserted into the global queue (`add_global_command`). Inserting before detaching corrupts the `next` pointer because `add_global_command` writes to `new_command->next`.

### Deferred promotion pattern

After executing a command, the next local command is not promoted immediately but collected in a `to_promote` list. All promotions happen after the current iteration of the global queue completes. This prevents a newly promoted command from being executed in the same `refresh_commands` call.

## 4.6 Running tests and coverage

```bash
# Run Criterion unit tests
make -C NET/tests tests_run

# Generate gcov HTML coverage report
make -C NET/tests coverage
# → opens coverage/index.html
```

Test files are in `NET/tests/`:
- `blacklist/test_blacklist.c`
- `map/test_map.c`
- `network/test_network_parsing.c`
- `parser/test_parser.c`
- `utils/test_utils.c`
- `world/test_world.c`

---

# 5. Game rules & world mechanics

## 5.1 The world — a torus grid

The map is a 2D grid of `width × height` tiles. The edges wrap: moving north from row 0 lands on row `height-1`, and moving east from column `width-1` lands on column 0. This is a torus topology. There are no walls and no edge effects.

Each tile holds:
- A count of each of the 7 resources.
- A linked list of players (and eggs) currently standing on it.

The minimum recommended map size is 10×10. Map size is fixed for the duration of a game.

## 5.2 Resources

There are 7 resource types. Food is the only one consumed by players; the six stones are used for incantation.

| Resource | Enum | Spawn density |
|---|---|---|
| Food | `FOOD` | 0.5 per tile (50% density) |
| Linemate | `LINEMATE` | 0.30 per tile |
| Deraumere | `DERAUMERE` | 0.15 per tile |
| Sibur | `SIBUR` | 0.10 per tile |
| Mendiane | `MENDIANE` | 0.10 per tile |
| Phiras | `PHIRAS` | 0.08 per tile |
| Thystame | `THYSTAME` | 0.05 per tile |

Density means: the target count of that resource on the entire map equals `density × width × height`. For example on a 20×20 map, the server targets 200 food units and 20 thystame units.

## 5.3 Resource respawn

The server schedules a `REFILL` pseudo-command every `20000 / freq` milliseconds. When it fires, `spawn_resources` checks `world->resource_counts[i]` against the target for each resource and places only the missing units, distributed via Fisher-Yates shuffle over all tiles. Existing resources are never removed by respawn.

## 5.4 Food and starvation

Every player starts with some food in inventory. Food is consumed at the rate of **1 unit per 126 ticks**. The server's `refresh_ais` function checks every live AI each tick using `last_check` timestamps. When `inventory[FOOD]` reaches 0, the server sends `"dead\n"` to the AI and destroys the player.

With `freq=10`: one tick = 100ms, so 1 food unit = 12.6 seconds of life.
With `freq=100`: one tick = 10ms, so 1 food unit = 1.26 seconds of life.

## 5.5 Player levels and incantation

Players start at level 1 and can reach level 8 (max). Leveling up requires gathering specific stones on a tile and having enough same-level players present, then issuing the `Incantation` command.

### Requirements per level

| Current level | Players needed | linemate | deraumere | sibur | mendiane | phiras | thystame |
|---|---|---|---|---|---|---|---|---|
| 1 | 1 | 1 | 0 | 0 | 0 | 0 | 0 |
| 2 | 2 | 1 | 1 | 1 | 0 | 0 | 0 |
| 3 | 2 | 2 | 0 | 1 | 0 | 2 | 0 |
| 4 | 4 | 1 | 1 | 2 | 0 | 1 | 0 |
| 5 | 4 | 1 | 2 | 1 | 3 | 0 | 0 |
| 6 | 6 | 1 | 2 | 3 | 0 | 1 | 0 |
| 7 | 6 | 2 | 2 | 2 | 2 | 2 | 1 |

All required stones must be placed on the tile (via `Set`) before calling `Incantation`. The initiating player calls `Incantation`; all other required players must simply be present on the tile at the same level — they do not need to issue any command. All eligible players are elevated simultaneously. Stones are consumed from the tile.

### Incantation sequence

```
AI (initiator)        Server                   Other AIs on tile
      │                  │                           │
      │── Incantation ──►│                           │
      │                  │──"Elevation underway"────►│ (to all on tile)
      │◄─"Elevation underway"                        │
      │                  │   (300 ticks pass)        │
      │◄─"Current level: N"                          │
      │                  │──"Current level: N" ─────►│
      │                  │──"pie X Y 1"─────────────►│ (to GUI)
```

If conditions are no longer met when the 300 ticks expire (a player left, stones taken), the server sends `ko` and `pie X Y 0` to the GUI.

## 5.6 Egg lifecycle

When a player uses `Fork`, the server places an egg on the player's current tile. The egg is represented as an `ai_t` with `level == 0` and `egg_type == FORK`.

```
Player calls Fork
    │
    ▼
Egg appears on tile  (server → GUI: "enw <egg_id> <parent_id> X Y")
    │
    ▼
New AI client connects and sends a team name with available slots
    │
    ├── a FORK egg exists for that team → hatch it
    │       server → GUI: "ebo <egg_id>"
    │       egg becomes a level-1 player
    │
    └── no egg, but team has initial slots → bloom a new player
```

Initial eggs (type `INITIAL`) are created during world initialisation, one per `eggs_per_team` slot per team. They represent the starting population budget. Fork eggs represent dynamically bred players.

If an `Eject` is performed on a tile containing fork eggs, those eggs are **destroyed** (`edi <egg_id>` to GUI).

## 5.7 Ejection mechanics

`Eject` pushes all other players on the current tile one tile in the direction the ejector is facing. Ejected players:
- Are moved to the tile in front of the ejector.
- Receive `"eject: K"` where K is the direction they were pushed from (relative to their own facing).
- Can be ejected even during incantation, which cancels their elevation.

Fork eggs on the tile are destroyed (not moved).

The direction K sent to the ejected player is computed using the torus shortest vector from ejector to ejected player, then rotated by the ejected player's facing direction (not the ejector's).

## 5.8 Win condition

A team wins when **6 or more of its players simultaneously reach level 8**. The server then sends `"seg <team_name>\n"` to all connected GUIs and terminates. The server exit code is 0 on a clean win.

Players can reach level 8 simultaneously during the same incantation call (since multiple players are elevated at once).

## 5.9 The frequency parameter

`-t <freq>` sets the number of ticks per second. It affects:

- **Command execution speed**: a `Forward` costs 7 ticks = `7000 / freq` ms.
- **Food consumption**: 1 food = 126 ticks = `126000 / freq` ms of life.
- **Resource respawn**: every `20000 / freq` ms.

Higher frequency makes the game faster but also makes AIs starve faster. The frequency can be changed at runtime by the GUI via the `sst` command.

Typical values: `freq=10` (casual), `freq=100` (fast debugging), `freq=1` (very slow, step-through debugging).
