#
# EPITECH PROJECT, 2026
# Zappy
# File description:
# Server response handling
#

from AI.algo.state import State
from AI.algo.constants import OK_COMMANDS, PAYLOAD_COMMANDS, FOOD_DURATION

ELEVATION_UNDERWAY = "Elevation underway\n"


class ResponseHandler:

    def __init__(self, agent):
        self.agent = agent

    def handle(self, response, connection, mode):
        agent = self.agent

        if response is None:
            return False

        if response == "dead":
            connection.deconnexion()
            return False

        if not agent.queue:
            return True

        response = agent.broadcast.drain_broadcasts(response, connection)
        if response is None:
            return False

        current_command = agent.queue[0].strip()
        command_parts = current_command.split()
        base_command = command_parts[0]

        if response == ELEVATION_UNDERWAY and base_command != "Incantation":
            self._handle_figurant_elevation(connection)
            return True

        cost = OK_COMMANDS.get(base_command, PAYLOAD_COMMANDS.get(base_command, 1))
        agent.survival_time -= cost

        if base_command == "Incantation":
            self._handle_caster_incantation(response, connection)
        elif base_command == "Look":
            self._handle_look(response)
        elif base_command == "Take":
            self._handle_take(response, current_command)
        elif base_command == "Set":
            self._handle_set(response, command_parts)
        elif base_command == "Inventory" and response.startswith("["):
            self._handle_inventory(response)
        elif base_command == "Fork" and mode == "auto":
            self._handle_fork(response)

        agent.queue.popleft()
        return True

    def _handle_figurant_elevation(self, connection):
        agent = self.agent

        level_response = connection.receive()
        level_response = agent.broadcast.drain_broadcasts(level_response, connection)

        if not (level_response and "Current level" in level_response):
            return

        agent.level = int(level_response.split(":")[-1].strip())
        agent.vision = []
        agent.pending_moves.clear()

        if agent.is_leader:
            agent.state = State.LEADER
        else:
            agent.state = State.ARRIVED

        agent.pending_moves.append("Inventory\n")
        agent.pending_moves.append("Look\n")

    def _handle_caster_incantation(self, response, connection):
        agent = self.agent

        if response != ELEVATION_UNDERWAY:
            if agent.is_leader:
                print(f"[LEADER {agent.id}] tick={agent.turns} INCANTATION ECHOUEE immediatement, response={response!r}")
            return

        level_response = connection.receive()
        level_response = agent.broadcast.drain_broadcasts(level_response, connection)

        if not (level_response and "Current level" in level_response):
            if agent.is_leader:
                print(f"[LEADER {agent.id}] tick={agent.turns} INCANTATION ECHOUEE, level_response={level_response!r}")
            return

        agent.level = int(level_response.split(":")[-1].strip())

        if agent.is_leader:
            agent.state = State.LEADER
            agent.pending_moves.clear()
            agent.leader_stopped = False
            print(
                f"[LEADER {agent.id}] tick={agent.turns} INCANTATION REUSSIE -> nouveau level="
                f"{agent.level}, leader_stopped remis a False (relance appel des joueurs)"
            )

    def _handle_look(self, response):
        agent = self.agent
        agent.vision = agent.look_cmd.parse_look_response(response, agent.level)

        if not agent.first_look_done:
            agent.first_look_done = True
            current_tile = agent.vision[0][0] if isinstance(agent.vision[0], list) else agent.vision[0]
            
            if current_tile.count("player") > 1:
                agent.is_child = True
                agent.pending_moves.append("Forward\n")
            else:
                agent.is_child = False

    def _handle_take(self, response, current_command):
        agent = self.agent
        if response == "ok\n":
            agent.survival_time = agent.inventory.update_from_command(
                current_command, agent.survival_time
            )

    def _handle_set(self, response, command_parts):
        agent = self.agent
        if response != "ok\n":
            return
        item_set = command_parts[1]
        if agent.inventory.data.get(item_set, 0) > 0:
            agent.inventory.data[item_set] -= 1

    def _handle_inventory(self, response):
        agent = self.agent
        agent.inventory.parse(response)
        agent.survival_time = agent.inventory.data.get("food", 0) * FOOD_DURATION + 119

    def _handle_fork(self, response):
        import subprocess, sys
        agent = self.agent
        # spawn_args n'existe qu'en mode Bonus (set par Bonus.py avant start())
        # main.py classique n'est jamais affecte
        spawn_args = getattr(agent, "spawn_args", None)
        if response != "ok\n" or spawn_args is None:
            return
        subprocess.Popen(
            [sys.executable, "-m", "AI.Bonus"] + spawn_args,
            start_new_session=True
        )