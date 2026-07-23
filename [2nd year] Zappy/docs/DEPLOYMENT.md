# Deployment Guide

This document covers setting up Zappy for production use, testing at scale, and troubleshooting common deployment issues.

---

## 1. Production Server Setup

### 1.1 Hardware Requirements

| Component | Minimum | Recommended |
|-----------|---------|-------------|
| CPU | 2 cores | 4+ cores |
| RAM | 2 GB | 4 GB |
| Storage | 1 GB | 10 GB (for logs, replays) |
| Network | 100 Mbps | 1 Gbps (for many clients) |

### 1.2 Operating System

- **Linux** (Ubuntu 20.04 LTS or later recommended)
- Kernel 5.10+
- glibc 2.31+

### 1.3 Network Configuration

#### Port Configuration
```bash
# Choose an available port (e.g., 4242)
sudo ufw allow 4242/tcp      # UFW firewall
sudo firewall-cmd --permanent --add-port=4242/tcp  # Firewalld
```

#### Listening Address
- By default, the server listens on `0.0.0.0` (all interfaces)
- To restrict to localhost only: [TODO: Check if this is a build-time or CLI option]

#### Bandwidth Estimation
- Average command: ~50 bytes (sent + received)
- 100 players, 10 TPS: ~50 KB/sec per player = 5 MB/sec total
- Most of this is GUI push events (broadcast events)

### 1.4 Systemd Service File (Optional)

Create `/etc/systemd/system/zappy-server.service`:

```ini
[Unit]
Description=Zappy Game Server
After=network.target

[Service]
Type=simple
User=zappy
WorkingDirectory=/opt/zappy
ExecStart=/opt/zappy/zappy_server -p 4242 -x 50 -y 50 -n team1 team2 team3 -c 5 -f 100
Restart=on-failure
RestartSec=5

# Logging
StandardOutput=journal
StandardError=journal
SyslogIdentifier=zappy

# Resource limits
LimitNOFILE=65536
MemoryLimit=2G

[Install]
WantedBy=multi-user.target
```

Then:
```bash
sudo systemctl daemon-reload
sudo systemctl enable zappy-server
sudo systemctl start zappy-server
sudo systemctl status zappy-server
```

---

## 2. Client Deployment

### 2.1 AI Client Scaling

Deploy multiple AI instances across machines:

**On each machine:**
```bash
for i in {1..5}; do
    ./zappy_ai -h <server_ip> -p 4242 -n team1 &
done
wait
```

Or use a container/process manager:

```bash
# Using GNU Parallel
parallel -j 10 ./zappy_ai -h server -p 4242 -n team1 ::: {1..100}
```

### 2.2 GUI Client

The GUI requires a display. Deploy to:
- **Workstations** with X11/Wayland (direct run)
- **Remote machines** via SSH X-forwarding: `ssh -X user@host`
- **Headless servers** via VNC / Xvfb (not recommended due to performance)

---

## 3. Environment Configuration

### 3.1 Server Flags Reference

```bash
./zappy_server \
    -p <port>                    # Listen port (required, 1024–65535)
    -x <width>                   # Map width in tiles (required, 10–500)
    -y <height>                  # Map height in tiles (required, 10–500)
    -n <team1> [<team2>...]      # Team names (required, 1+ teams)
    -c <eggs_per_team>           # Starting eggs per team (default: 1)
    -t <frequency>               # Game frequency in ticks/sec (default: 100)
    -firewall                    # Enable IP banning on disconnect (optional)
    -v                           # Verbose logging (optional)
```

### 3.2 Tuning for Different Scenarios

#### Fast-Paced Game (Teaching)
```bash
./zappy_server -p 4242 -x 20 -y 20 -n team1 team2 -c 3 -t 150
```
- Smaller map = quicker matches
- Faster ticks = snappier response

#### Large-Scale Simulation (Research)
```bash
./zappy_server -p 4242 -x 100 -y 100 -n team1 team2 team3 -c 10 -t 50
```
- Larger map = more strategic depth
- More players = more coordination

#### Single-Team Tutorial
```bash
./zappy_server -p 4242 -x 15 -y 15 -n solo -c 5 -t 75
```

### 3.3 Resource Spawning

The server distributes food and minerals across the map at server startup. The spawn density depends on map size:

```
Total resources ≈ map_width × map_height × density_factor
```

[TODO: Confirm exact resource spawning algorithm and any CLI flags to control it]

---

## 4. Monitoring and Logging

### 4.1 Systemd Logs

```bash
# Follow live logs
journalctl -f -u zappy-server

# Last 100 lines
journalctl -u zappy-server -n 100

# Since last boot
journalctl -u zappy-server --since today
```

### 4.2 Manual Logging (Verbose Mode)

```bash
./zappy_server -v -p 4242 -x 20 -y 20 -n team1 -c 3 | tee zappy.log
```

### 4.3 Performance Metrics

Monitor using standard Linux tools:

```bash
# CPU and memory usage
top -p <server_pid>

# Network traffic
nethogs -d eth0

# Open connections
netstat -an | grep :4242 | wc -l
```

---

## 5. Troubleshooting Deployment

### 5.1 "Address already in use"

Another process is using the port:
```bash
lsof -i :4242
kill -9 <pid>
# Or use a different port: -p 4243
```

### 5.2 "Cannot bind to address"

Possible causes:
- Port number < 1024 (requires root)
- Network interface not available
- Firewall blocking

**Solution:**
```bash
# Use a port >= 1024
./zappy_server -p 4242 ...

# Check firewall
sudo ufw status
sudo ufw allow 4242/tcp
```

### 5.3 "Memory allocation failed"

Happens on very large maps:
- Reduce map size: `-x 80 -y 80` instead of `-x 200 -y 200`
- Add swap space: `sudo fallocate -l 2G /swapfile && sudo chmod 600 /swapfile && sudo mkswap /swapfile && sudo swapon /swapfile`
- Upgrade RAM

### 5.4 Slow Gameplay / Lag

**Symptoms:** Commands take seconds to execute, GUI freezes

**Causes & Solutions:**
1. Server CPU overloaded
   - Reduce player count: `./zappy_ai` instances
   - Reduce frequency: `-t 50` instead of `-t 100`
   - Use faster hardware

2. Network latency
   - Check network: `ping <server_ip>` should be < 10ms
   - Use a server closer to clients
   - Upgrade network to lower-latency service

3. GUI rendering bottleneck
   - Reduce map size viewed (zoom out in GUI)
   - Run GUI on a faster machine
   - Disable animations/shadows [TODO: Confirm GUI options]

### 5.5 AI Clients Disconnecting

**Symptoms:** Players disappear after ~30 seconds

**Causes & Solutions:**
1. AI not sending commands (network timeout)
   - Check AI logs: `./zappy_ai ... 2>&1 | tee ai.log`
   - Verify server is responsive: `nc -zv <server_ip> 4242`

2. Server closing connection (starvation)
   - Increase starting food on server
   - Reduce player count

3. Firewall/NAT issues
   - If AI and server on different networks, ensure firewall allows port
   - [TODO: Add firewall mode info if applicable]

### 5.6 GUI Can't Connect

**Symptoms:** "Connection failed" immediately upon trying to connect

**Debugging:**
```bash
# Test server is listening
telnet <server_ip> 4242

# Check firewall from client machine
sudo ufw allow in on eth0 to any port 4242 from <client_ip>

# Test with nc
nc -zv <server_ip> 4242   # should say "succeeded"
```

---

## 6. Scaling to Multiple Servers

For a tournament or large-scale deployment with 500+ players, run multiple servers on different ports:

```bash
# Server 1 (Tournament bracket A)
./zappy_server -p 4242 -x 50 -y 50 -n redA blueA greenA -c 5 -f 100

# Server 2 (Tournament bracket B)
./zappy_server -p 4243 -x 50 -y 50 -n redB blueB greenB -c 5 -f 100

# Server 3 (etc.)
./zappy_server -p 4244 ...
```

Each server is independent. Results can be aggregated by an external system.

---

## 7. Database / Replay System

[TODO: If replay/logging is implemented, document how to:
- Enable replay recording
- List replays
- Replay a recorded game
- Export replay data]

---

## 8. Backup and Recovery

### 8.1 Backup Strategy

Important files to back up:
- Binary builds: `/opt/zappy/zappy_server`, `zappy_ai`, `zappy_gui` (build from source if lost)
- Configuration: [TODO: Document any config files]
- Logs: `zappy.log` (optional, for diagnostics)

```bash
tar -czf zappy-backup-$(date +%Y%m%d).tar.gz /opt/zappy
```

### 8.2 Disaster Recovery

If the server crashes:
```bash
systemctl restart zappy-server
# or manually:
./zappy_server -p 4242 ... &
```

The server has no persistent state; a restart begins a fresh game.

---

## 9. Security Considerations

### 9.1 Network

- **Plaintext protocol**: Use on trusted LANs only. For untrusted networks, tunnel through SSH or VPN.
- **No authentication**: Any client can connect and join a team. Use firewall rules or VPN for access control.
- **No encryption**: Broadcasts and commands are readable in transit.

### 9.2 DoS Prevention

The server has no built-in rate limiting. Mitigate DoS via:
- Firewall (iptables, UFW)
- Load balancer (nginx, haproxy)
- Bandwidth limiting (tc qdisc)

### 9.3 Firewall Rules (UFW)

```bash
# Allow server port from specific subnet
sudo ufw allow from 192.168.1.0/24 to any port 4242

# Deny all except from trusted IPs
sudo ufw default deny
sudo ufw allow from 192.168.1.5 to any port 4242
```

---

## 10. Performance Tuning

### 10.1 Linux Kernel Tuning

For handling many concurrent connections:

```bash
# Increase max open files
ulimit -n 65536

# Increase TCP backlog
sysctl -w net.core.somaxconn=4096
sysctl -w net.ipv4.tcp_max_syn_backlog=4096
```

### 10.2 Application Tuning

- **Frequency**: `-t 100` is good for most uses; `-t 50` for very large games (saves CPU)
- **Eggs**: `-c 10` allows more players to spawn; uses more memory
- **Map size**: Larger = more resource spawning points, more strategic gameplay

---

## Checklist: Pre-Production Deployment

- [ ] Server binaries built and tested
- [ ] Port 4242 (or chosen port) is open in firewall
- [ ] Systemd service file created (if using systemd)
- [ ] Monitoring / logging is configured
- [ ] Backup strategy documented
- [ ] Security rules in place (firewall, VPN, etc.)
- [ ] Load testing completed (at least 50 concurrent players)
- [ ] Documentation linked from team wiki or README
- [ ] Runbook for common issues created
- [ ] On-call rotation assigned (if applicable)

---

**Questions?** See [docs/CONTRIBUTING.md](./CONTRIBUTING.md) for how to report issues or request features.
