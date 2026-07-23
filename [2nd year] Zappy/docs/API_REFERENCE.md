# Zappy Network Protocol Reference

This document provides a complete reference of all commands and messages in the Zappy network protocol.

---

## 1. Connection Handshake

### 1.1 AI Client Handshake

```
SERVER ──→ CLIENT   "WELCOME\n"
CLIENT ──→ SERVER   "<team_name>\n"
SERVER ──→ CLIENT   "<available_slots>\n"
SERVER ──→ CLIENT   "<map_width> <map_height>\n"
```

**Example:**
```
Server sends:  WELCOME
Client sends:  team_red
Server sends:  5
Server sends:  30 30
```

### 1.2 GUI (Spectator) Handshake

```
SERVER ──→ GUI   "WELCOME\n"
GUI ──→ SERVER   "GRAPHIC\n"
```

After this, the GUI can send query commands and receives push events asynchronously.

### 1.3 Errors

If a team is full, the server closes the connection without sending `<available_slots>`.

```
SERVER ──→ CLIENT   "Team is full\n"
[connection closes]
```

---

## 2. AI Commands

### 2.1 Reference Table

| Command | Arguments | Response (success) | Response (failure) | Ticks | Notes |
|---------|-----------|-------|--------|-------|-------|
| `Forward` | — | `ok` | — | 7 | Move forward in current direction |
| `Right` | — | `ok` | — | 7 | Turn 90° right (does not move) |
| `Left` | — | `ok` | — | 7 | Turn 90° left (does not move) |
| `Look` | — | `[tiles...]` | — | 7 | See tiles in view cone |
| `Inventory` | — | `[resources...]` | — | 1 | Get current inventory |
| `Broadcast <text>` | `<text>` (1–200 chars) | `ok` | — | 7 | Send message to team |
| `Connect_nbr` | — | `<N>` (integer) | — | 0 | Query free player slots |
| `Take <resource>` | `<resource>` | `ok` | `ko` | 7 | Pick up resource from tile |
| `Set <resource>` | `<resource>` | `ok` | `ko` | 7 | Drop resource on tile |
| `Incantation` | — | `Current level: <N>` | `ko` | 300 | Start level-up ritual |
| `Fork` | — | `ok` | — | 42 | Lay egg (creates new player) |
| `Eject` | — | `ok` | `ko` | 7 | Remove all other players from tile |

### 2.2 Command Details

#### `Forward`
Move one tile forward in the player's current direction.
- **Success**: Server responds `ok`, player's position updates
- **Failure**: Never fails (stays in place if blocked by edge, but wraps via torus)
- **Cost**: 7 ticks

#### `Right` / `Left`
Turn 90° without moving.
- **Directions**: 0=North, 1=East, 2=South, 3=West
- **Cost**: 7 ticks each

#### `Look`
Get a snapshot of the current view cone (front-facing tiles).

**Response format:**
```
[resource_list, resource_list resource_list resource_list, ...]
```

Where each `resource_list` is a space-separated list of resources at that distance. Distances expand: 1 tile away (1 tile), 2 away (3 tiles), 3 away (5 tiles), etc.

**Example** (level-2 agent looking north, sees linemate 1 tile ahead, food 2 tiles ahead):
```
[linemate] [food] [] []
```

Each player can see `level + 1` rows. Max vision at level 7 is 8 rows / 64 tiles.

#### `Inventory`
List all resources in your inventory with counts.

**Response format:**
```
[linemate N, deraumere N, sibur N, mendiane N, phiras N, thystame N, food N]
```

**Example:**
```
[linemate 2, deraumere 0, sibur 1, mendiane 0, phiras 0, thystame 0, food 5]
```

#### `Broadcast <text>`
Send a message to all players on your team (and direction-encoded to those who hear it).

**Response**: `ok`

**Server → Other Players**:
```
message K, <text>
```

Where `K` is the direction (0–8) to the broadcaster.

#### `Connect_nbr`
Query how many egg slots are still available for your team.

**Response**: `<N>` where N is the number of free slots.

**Zero cost** — this command never consumes ticks.

#### `Take <resource>`
Pick up a resource from your current tile.

**Resources**: `food`, `linemate`, `deraumere`, `sibur`, `mendiane`, `phiras`, `thystame`

**Success**: `ok` (inventory updates on next `Inventory` command)

**Failure**: `ko` if resource not present or inventory full (10 items max)

#### `Set <resource>`
Drop a resource from your inventory onto your current tile.

**Success**: `ok`

**Failure**: `ko` if you don't have that resource

#### `Fork`
Create a new unhatched egg on your current tile.

**Cost**: 42 ticks

**Mechanic**: The server immediately places an egg on the tile. When the egg hatches (after ~5 game ticks typically), a new player joins the team.

#### `Incantation`
Attempt to level up. Requires:
1. Specific resources held in inventory (see table in [Architecture](./ARCHITECTURE.md))
2. Other players (6+) at the same level on the same tile
3. No other players at different levels on the same tile

**Response when started**: `Elevation underway`

**Response when complete (300 ticks later)**: `Current level: <N>` where N is the new level

**Failure response**: `ko` if conditions not met (missing resources, wrong players, etc.)

**Side effects**: 
- All resources specified in the incantation are consumed
- All other players at the same level on the same tile are also elevated
- Non-food items are ejected from the tile afterward

#### `Eject`
Remove all other players from your current tile.

**Response**: `ok` (always succeeds)

**Side effects**: 
- All other players on your tile are displaced to an adjacent tile
- Each ejected player receives: `eject: K` where K is the direction (1–8) they were ejected toward
- Fork eggs (but not food/stones) are destroyed

---

## 3. GUI Commands

### 3.1 Query Commands (GUI → Server)

The GUI can send any of these commands at any time and receive an immediate response.

| Command | Arguments | Response | Example |
|---------|-----------|----------|---------|
| `msz` | — | `msz <W> <H>` | `msz 30 30` |
| `bct` | `<X> <Y>` | `bct <X> <Y> <f> <l> <d> <s> <m> <p> <t>` | `bct 5 10 2 1 0 3 0 1 0` |
| `mct` | — | (one `bct` line per tile) | (60–900 lines) |
| `tna` | — | (one `tna <name>` line per team) | `tna team1` `tna team2` |
| `ppo` | `<ID>` | `ppo <ID> <X> <Y> <dir>` | `ppo 42 10 15 2` |
| `plv` | `<ID>` | `plv <ID> <level>` | `plv 42 3` |
| `pin` | `<ID>` | `pin <ID> <X> <Y> <f> <l> <d> <s> <m> <p> <t>` | `pin 42 10 15 0 2 1 0 0 0 2` |
| `sgt` | — | `sgt <freq>` | `sgt 100` |
| `sst` | `<freq>` | `sst <freq>` (also **sets** server frequency) | `sst 50` |

#### Response Details

**`msz <W> <H>`**: Map size in tiles

**`bct <X> <Y> <food> <linemate> <deraumere> <sibur> <mendiane> <phiras> <thystame>`**: Board content at one tile

**`mct`**: Outputs one `bct` line for every tile on the map (left-to-right, top-to-bottom)

**`tna <team_name>`**: Team names (one line per team)

**`ppo <ID> <X> <Y> <dir>`**: Player position and direction
- Direction: 1=North, 2=East, 3=South, 4=West

**`plv <ID> <level>`**: Player level (0=egg, 1–8=alive)

**`pin <ID> <X> <Y> <food> ...`**: Player inventory at position (X, Y)

**`sgt <freq>`**: Server game frequency (ticks/second)

### 3.2 Server Push Events (Server → GUI)

These arrive asynchronously whenever something happens in the game:

| Event | Format | Trigger |
|-------|--------|---------|
| `pnw` | `pnw <ID> <X> <Y> <dir> <level> <team>` | New player connects |
| `ppo` | `ppo <ID> <X> <Y> <dir>` | Player moves |
| `plv` | `plv <ID> <level>` | Player levels up |
| `pin` | `pin <ID> <X> <Y> <f> <l> <d> <s> <m> <p> <t>` | Inventory change |
| `pex` | `pex <ID>` | Player ejects other players |
| `pbc` | `pbc <ID> <text>` | Player broadcasts message |
| `pic` | `pic <X> <Y> <level> <ID1> <ID2> ...` | Incantation starts (all participants listed) |
| `pie` | `pie <X> <Y> <result>` | Incantation ends (1=success, 0=failed) |
| `pfk` | `pfk <ID>` | Player forks (egg laid) |
| `enw` | `enw <ID> <parent_id> <X> <Y>` | New egg spawned |
| `ebo` | `ebo <ID>` | Egg hatches (becomes player) |
| `edi` | `edi <ID>` | Egg destroyed (e.g., by eject) |
| `pdi` | `pdi <ID>` | Player dies (starvation) |
| `pdr` | `pdr <ID> <resource>` | Player drops resource |
| `pgt` | `pgt <ID> <resource>` | Player picks up resource |
| `seg` | `seg <team>` | Game ends; team wins |

---

## 4. Broadcast Direction Algorithm

When a player broadcasts, other players receive a direction code `K` (0–8) indicating where the sender is:

```
K=0: Same tile
K=1: North
K=2: Northeast  
K=3: East
K=4: Southeast
K=5: South
K=6: Southwest
K=7: West
K=8: Northwest
```

The direction is computed relative to the **receiver's facing direction**, not absolute. This requires computing the shortest path on the torus and rotating by the receiver's heading.

---

## 5. Resource Types

The game has 7 types of resources:

1. `food` — Sustenance; consumed each tick; players die without it
2. `linemate` — Mineral type 1
3. `deraumere` — Mineral type 2
4. `sibur` — Mineral type 3
5. `mendiane` — Mineral type 4
6. `phiras` — Mineral type 5
7. `thystame` — Mineral type 6

Each incantation requires specific quantities at each level (see [ARCHITECTURE.md](./ARCHITECTURE.md#level-up-resource-requirements)).

---

## 6. Error Handling

### 6.1 Connection Errors

| Scenario | Response |
|----------|----------|
| Unknown command | `ko` |
| Team doesn't exist | Server closes connection |
| Team is full | `Team is full`, server closes |
| Server is at capacity | `Server is full`, server closes |
| Client is banned | `You're banned from this server`, server closes |
| Malformed command | `ko` (if parseable) or connection closes |

### 6.2 Command Errors

Commands that fail (insufficient resources, wrong state) respond `ko`. The command is **still consumed** (costs ticks).

---

## 7. Pipelining and Queuing

### 7.1 AI Pipelining

Each AI can have **up to 10 queued commands**. Once a command starts executing, subsequent commands wait in a local queue:

```
AI sends: Forward Forward Forward Forward Forward
          ↓
Server: Executes 1st, queues 2nd–5th locally
```

Sending more than 10 commands at once causes the oldest to be dropped silently (flow control).

### 7.2 GUI Non-Blocking

GUI commands are stateless queries and return immediately. GUI can send and receive concurrently.

---

## 8. Timing Examples

### 8.1 Server Frequency

If the server is running at 100 ticks/second:
- 1 tick = 10 milliseconds
- A 7-tick command takes 70 milliseconds

If the server is running at 10 ticks/second:
- 1 tick = 100 milliseconds
- A 7-tick command takes 700 milliseconds

The GUI can query `sgt` to get the current frequency and `sst` to change it.

### 8.2 Incantation Timeline

```
T=0:     Incantation command received
T=1-299: Server processes, GUI receives "pic" event
T=300:   Server sends "pie 1" (success) or "pie 0" (fail)
```

All 6+ players at the same level on the same tile are elevated simultaneously.

---

## 9. Example Session

### Scenario: Two-player incantation at level 2

**Server setup:**
```bash
./zappy_server -p 4242 -x 20 -y 20 -n team1 -c 2 -f 100
```

**AI 1 (Player 100):**
```
Forward            # ok (7 ticks)
Take linemate      # ok
Set linemate       # ok
Inventory          # [linemate 0, ..., food 5]
Incantation        # [waits for Player 101]
```

**AI 2 (Player 101):**
```
Forward
Left
Forward            # moves to same tile as Player 100
Inventory          # [linemate 0, ..., food 5]
Take linemate
Set linemate
Incantation        # triggers incantation
```

**Server timeline:**
```
T=100: Both players issue Incantation (same tile, both level 1→2, resource check passes)
T=101: Send to GUI: "pic 5 5 2 100 101"
T=300: Send to AI 100/101: "Current level: 2"
       Send to GUI: "pie 5 5 1" (success)
       Broadcast stones are ejected
```

---

## 10. Common Integration Patterns

### 10.1 Polling Loop (GUI)

```python
while running:
    send_query("sgt")           # Get server frequency
    handle_query_responses()
    receive_push_events()       # Handle pnw, ppo, etc.
    render_frame()
```

### 10.2 Client Loop (AI)

```python
while connected:
    read_server_response()      # ok, ko, [resources], message, etc.
    update_internal_state()
    compute_next_command()
    send_command()
```

---

**Next step:** See [ARCHITECTURE.md](./ARCHITECTURE.md) for system design and [CONTRIBUTING.md](./CONTRIBUTING.md) for development guidelines.
