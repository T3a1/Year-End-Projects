# Getting Started with Zappy

This guide walks you through installing, building, and running your first Zappy game.

---

## 1. Prerequisites

### System Requirements
- **Operating System**: Linux with a graphical desktop (X11 or Wayland)
- **Processor**: Any modern CPU (tested on Intel/AMD)
- **RAM**: Minimum 2GB; 4GB+ recommended
- **Disk Space**: ~500MB for source + build artifacts

### Required Software

| Tool | Version | Purpose |
|------|---------|---------|
| `gcc` or `clang` | C17 capable | Compile the C server |
| `g++` or `clang++` | C++17 capable | Compile the C++ GUI |
| `make` | 4.0+ | Build orchestration |
| `cmake` | 3.10+ | GUI build configuration |
| `python` | 3.7+ | AI client runtime |
| `git` | any | Clone the repository (optional) |

### Graphics Library: Raylib

The GUI requires **Raylib**, a simple graphics library. Install it one of two ways:

#### Option A: System Package (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install libraylib-dev
```

#### Option B: Build from Source (Recommended)
```bash
git clone https://github.com/raysan5/raylib.git
cd raylib
mkdir build && cd build
cmake .. -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
sudo make install
```

After installation, verify raylib is found:
```bash
pkg-config --cflags --libs raylib
```

---

## 2. Clone or Extract the Repository

If you have a Git repository:
```bash
git clone <repository-url> Zappy
cd Zappy
```

Or if you have a `.zip` archive:
```bash
unzip Zappy.zip
cd Zappy
```

---

## 3. Build the Project

From the **Zappy root directory**, run:

```bash
make all
```

This builds three components:

1. **`zappy_server`** — The game server (NET/), written in C
2. **`zappy_gui`** — The graphical client (GUI/), written in C++/Raylib
3. **`zappy_ai`** — The AI player launcher script (AI/)

All three binaries are placed in the repository root once the build completes.

### Build Troubleshooting

**CMake not found:**
```bash
sudo apt-get install cmake
```

**Raylib headers/library not found:**
```
CMake Error at GUI/CMakeLists.txt:5 (find_package):
  Could not find a configuration file for package "raylib".
```
→ Install raylib using Option B above (build from source).

**Permission denied on `zappy_gui`:**
```bash
chmod +x zappy_gui
```

**Compiler errors (C++17):**
Ensure your compiler is recent enough:
```bash
g++ --version          # should be 7.0+
clang++ --version      # should be 5.0+
```

---

## 4. Run Your First Game

You need **three terminal windows** open simultaneously. Each window runs one component.

### Terminal 1: Start the Server

```bash
./zappy_server -p 4242 -x 20 -y 20 -n team1 team2 -c 4 -f 100
```

**Flags:**
- `-p 4242` : Listen on port 4242 (use any free port)
- `-x 20 -y 20` : Map is 20×20 tiles
- `-n team1 team2` : Two teams named "team1" and "team2"
- `-c 4` : Start with 4 eggs per team
- `-f 100` : Game frequency = 100 ticks per second

**Expected output:**
```
Server started on port 4242
Waiting for connections...
```

### Terminal 2: Start the GUI

```bash
./zappy_gui -p 4242 -h localhost
```

**Flags:**
- `-p 4242` : Connect to server on port 4242 (must match Terminal 1)
- `-h localhost` : Server hostname/IP (localhost = same machine)

**Expected output:**
- A window titled "Zappy GUI" appears
- A pre-game menu shows "Port" and "IP" input fields
- Fields are pre-filled with "4242" and "localhost"
- A lock icon appears on the right (indicating connected)

If lock icon shows ❌ instead:
- Verify Terminal 1 is still running
- Check firewall isn't blocking port 4242
- Try `telnet localhost 4242` from another terminal to confirm the server is listening

### Terminal 3+: Start AI Players

Each terminal runs one AI player. Start at least one:

```bash
./zappy_ai -h localhost -p 4242 -n team1
```

**Flags:**
- `-h localhost` : Server hostname (same as GUI)
- `-p 4242` : Server port (must match)
- `-n team1` : Join this team (must be a team from `-n` in Terminal 1)

**Expected output:**
```
Connected to server on localhost:4242
Joining team: team1
Player ID: 12345
```

**Add more players** by opening more terminals and repeating the command (or changing `-n team2` for the other team):

```bash
./zappy_ai -h localhost -p 4242 -n team2
```

---

## 5. Watch the Simulation

Once you have at least one AI player connected, the GUI should show:

- **3D World**: A top-down view of the map
- **Players**: Green/blue characters (depending on team)
- **Resources**: Colored mineral blocks on the ground
- **UI Panels**: Right sidebar showing player list, tile info, settings

**Controls in the GUI:**
- **Mouse**: Click buttons on the UI to toggle panels
- **P**: Toggle player list
- **G**: Toggle grid overlay
- **I**: Toggle tile info panel
- **Scroll/Arrow Keys**: Rotate camera (if not hovering UI)
- **ESC**: Quit

Players will:
1. Move around the map gathering food and minerals
2. Broadcast messages to their team (visible in the chat panel)
3. Perform incantations when coordinated (you'll see an animation)
4. Level up and try to reach level 8

The game ends when one team has all players at level 8. The GUI will display the winner.

---

## 6. Advanced: Run with More Players

Create a more interesting game with more players and better AI coordination:

**Terminal 1:**
```bash
./zappy_server -p 4242 -x 50 -y 50 -n red blue green -c 5 -f 150
```

**Terminal 2:**
```bash
./zappy_gui -p 4242 -h localhost
```

**Terminals 3–12:** (10 AI players total)
```bash
./zappy_ai -h localhost -p 4242 -n red      # run this 4 times
./zappy_ai -h localhost -p 4242 -n blue     # run this 3 times
./zappy_ai -h localhost -p 4242 -n green    # run this 3 times
```

---

## 7. Advanced: AI Auto-Forking (Bonus)

If you want the AI to automatically spawn new players (bonus feature):

1. Use `zappy_ai_bonus` instead of `zappy_ai`:
   ```bash
   make zappy_ai_bonus
   ./zappy_ai -h localhost -p 4242 -n team1
   ```

2. The AI will detect available egg slots and spawn sibling processes automatically.

---

## 8. Troubleshooting

### "Server is full"
- Increase `-c` (eggs per team) on the server
- Or connect fewer players

### "Team doesn't exist"
- Double-check `-n` team names on the server
- Spelling and case must match exactly

### GUI shows "Connection failed"
- Verify firewall isn't blocking the port: `sudo ufw allow 4242`
- Check server is still running
- Try local IP instead of "localhost": `./zappy_gui -p 4242 -h 127.0.0.1`

### AI player gets "dead" immediately
- Server is not spawning enough food
- Try `-f` with a lower number (slower game = more food accumulation)
- Example: `-f 50` instead of `-f 100`

### GUI window won't open
- Verify you have a graphical session: `echo $DISPLAY` should return something like `:0`
- If running over SSH, use X-forwarding: `ssh -X user@host`

### Build fails: "raylib not found"
```bash
# Re-install raylib from source:
rm -rf ~/raylib
git clone https://github.com/raysan5/raylib.git ~/raylib
cd ~/raylib && mkdir build && cd build
cmake .. -DBUILD_SHARED_LIBS=OFF
make && sudo make install
sudo ldconfig   # Update library cache
cd ~/Zappy && make clean && make all
```

---

## 9. Next Steps

- **For Developers**: See [docs/ARCHITECTURE.md](./ARCHITECTURE.md) for internals and design patterns
- **For Recruiters**: Check [README.md](../README.md) for project overview and feature highlights
- **For Contributors**: Read [docs/CONTRIBUTING.md](./CONTRIBUTING.md) for code standards and PR process
- **For Protocol Details**: See [docs/API_REFERENCE.md](./API_REFERENCE.md)

---

## 10. Common Questions

**Q: Can I run everything on one machine?**  
A: Yes! Use `localhost` (or `127.0.0.1`) for all connections.

**Q: Can I run on a remote server?**  
A: Yes, but GUI requires X-forwarding: `ssh -X user@host`

**Q: Can I write my own AI client?**  
A: Absolutely! See [docs/API_REFERENCE.md](./API_REFERENCE.md) for the protocol.

**Q: How many players can I have?**  
A: Limited by memory. Tested up to 100+ concurrent players on a modern machine.

**Q: Is the AI always the same?**  
A: The Python AI in `AI/` is deterministic but includes randomized starting positions and some stochastic behavior. For fully custom AI, implement the protocol yourself.

---

**Happy gaming! 🎮**
