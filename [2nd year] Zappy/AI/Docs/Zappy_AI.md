# Zappy AI — Documentation

> Epitech 2026

This document covers only the AI client. For the network protocol it speaks
to, see the main project documentation (section "Protocol & command
reference").

---

## Table of contents

1. [Architecture](#1-architecture)
2. [Installation & running](#2-installation--running)
3. [AI strategy guide](#3-ai-strategy-guide)

---

# 1. Architecture

## 1.1 Process model

Each AI process is one Python interpreter controlling exactly one player on
the server. There is no shared memory between AI processes — every
coordination between players of the same team happens exclusively through
the server's `Broadcast` mechanism.

```
 zappy_ai (process 1) <--TCP--> NET server <--TCP--> zappy_ai (process 2)
 zappy_ai (process 3) <--TCP--> NET server <--TCP--> zappy_ai (process N)
```

There are two entry points, sharing the same core logic:

| Entry point | File | Behaviour |
|---|---|---|
| `zappy_ai` | `main.py` | Connects once, plays, never forks a sibling process itself |
| `zappy_ai` (bonus build) | `Bonus.py` | Same as above, plus: on connect, checks the free-slot count returned by the server and spawns a sibling process via `subprocess.Popen` if slots are already open |

Both entry points parse the same three CLI flags (`-p`, `-n`, `-h`, see
section 2), connect via `COM_SERVER`, and hand off to the same `Algo`
instance once the handshake with the server is done.

## 1.2 Module layout

```
AI/
├── main.py                  entry point (standard build)
├── Bonus.py                 entry point (auto-fork build)
├── com_server.py            COM_SERVER: TCP socket wrapper
├── requirements.txt         empty - stdlib only, no external dependency
├── algo/
│   ├── algo.py              Algo: top-level state machine driver
│   ├── state.py             State enum
│   ├── constants.py         all numeric constants and requirement tables
│   ├── inventory.py         Inventory: resource counters + food tracking
│   ├── level_up.py          LevelUp: level-1 solo collect/drop/incant cycle
│   ├── election.py          ElectionManager: leader election by lowest id
│   ├── leader.py            LeaderManager: farm/call/stabilize/incant cycle
│   ├── rally.py             RallyManager: follower movement toward the leader
│   ├── broadcast.py         BroadcastParser: parses incoming broadcasts
│   ├── survival.py          SurvivalManager: food-seeking behaviour
│   └── response_handler.py  ResponseHandler: server response dispatch
└── commande/
    └── look/
        └── look.py          Look: vision parsing + pathfinding
```

## 1.3 Responsibility split

```
Layer               | Responsibility
--------------------|---------------------------------------------------
main.py / Bonus.py  | CLI parsing, connection setup, process lifecycle
com_server.py       | Raw TCP I/O: connect, send, line-buffered receive
algo.py             | Owns all per-agent state, dispatches to managers
                    | below based on agent.level and agent.state
election.py         | Decides who is leader
leader.py           | Drives the elected leader's behaviour
rally.py            | Drives a follower's behaviour while rallying
survival.py         | Drives food-seeking behaviour outside any role
level_up.py         | Drives the level-1 solo path (no role involved)
broadcast.py        | Parses every incoming Broadcast and routes it to
                    | the relevant manager (election / leader / rally)
inventory.py        | Tracks resource counts and the survival_time budget
look/look.py        | Parses Look responses, computes movement plans
response_handler.py | Reads server replies and updates state accordingly
```

`Algo` (in `algo.py`) is the only class instantiated directly by the entry
points; every other class listed above is owned by it and reached through an
attribute of the same name (`self.election`, `self.leader`, `self.rally`,
`self.survival`, `self.broadcast`, `self.inventory`, `self.look_cmd`,
`self.level_up`, `self.response_handler`).

## 1.4 Per-tick control flow

```
Algo.start()
  loop while connected:
    command = next_move()        - decide one command based on state
    send_command(command)
    response = receive()
    handle_response(response)     - update state from the server reply
                                    + drain any queued pending_moves the same way
    turns += 1
```

`next_move()` is the single decision point: it reads `self.level` and
`self.state`, and delegates to `level_up` (level 1), `leader` (elected
leader), `rally` (`ARRIVED` / `RALLIED`), or `survival` (everything else).

---

# 2. Installation & running

## 2.1 Prerequisites

| Tool | Why |
|---|---|
| Python ≥ 3.10 | Runs the AI client |
| A running `zappy_server` | The AI has nothing to connect to otherwise |

The AI client only uses the Python standard
library (`socket`, `argparse`, `subprocess`, `collections`, `enum`,
`random`). No `pip install` step is needed.

## 2.2 Building

From the project root:

```bash
make zappy_ai          # copies main.py to ./zappy_ai, chmod +x
make zappy_ai_bonus    # copies Bonus.py to ./zappy_ai_bonus (auto-fork build), chmod +x
```

## 2.3 Running

```bash
./zappy_ai -h <machine_ip> -p <port> -n <team_name>
```

| Flag | Meaning | Required | Default |
|---|---|---|---|
| `-p` | Port the server is listening on | yes | — |
| `-n` | Team name (must match a team declared on the server) | yes | — |
| `-h` | Server machine address | no | `localhost` |

Example, connecting to a local server on port 4242 as team `red`:

```bash
./zappy_ai -h 127.0.0.1 -p 4242 -n red
```

`--help` (or `-help`) prints the usage line and exits immediately, before
any connection attempt:

```
USAGE: ./zappy_ai -p port -n name -h machine

option              description
-p port             port number
-n name             name of the team
-h machine          name of the machine; localhost by default
```

## 2.4 Exit behaviour

- If the connection is refused, times out, or the address can't be
  resolved, the process prints an `[ERREUR]` line and exits without ever
  reaching the game loop.
- If the team is full or the server itself is full, the server closes the
  connection right after the handshake and the AI exits.
- During the game, if the agent's food reaches 0, the server sends `dead`
  and the AI disconnects cleanly.
- `[INFO]` lines on startup (`Slots`, `Map`) are informational only and do
  not indicate any error.

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
