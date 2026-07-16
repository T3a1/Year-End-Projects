#
# EPITECH PROJECT, 2026
# Zappy
# File description:
# Broadcast parsing (Hello_, Leader_, RushLvl8_ messages)
#

RUSH_MIN_SURVIVAL_TIME = 3700
from AI.algo.state import State


class BroadcastParser:


    def __init__(self, agent):
        self.agent = agent

    def drain_broadcasts(self, response, connection):

        while response and response.startswith("message"):
            self.parse_broadcast(response)
            response = connection.receive()
        return response

    def parse_broadcast(self, message):
        direction, text = self._split_message(message)
        if text is None:
            return

        # Filtrer les messages qui ne sont pas de notre team
        team_prefix = f"[{self.agent.team_name}]"
        if not text.startswith(team_prefix):
            return
        text = text[len(team_prefix):]

        if text.startswith("RushLvl8_"):
            self._handle_rush_lvl8(direction, text)
        elif self.agent.level >= 2 and text.startswith("Hello_"):
            self._handle_hello(text)
        elif self.agent.level >= 2 and text.startswith("Leader_"):
            self._handle_leader(text)

    def _split_message(self, message):
        """Un message broadcast a la forme 'message <direction>, <texte>'.
        Retourne (direction:int|None, texte:str|None)."""
        parts = message.split(",")
        if len(parts) < 2:
            return None, None

        text = parts[1].strip()

        direction = None
        try:
            direction = int(parts[0].split()[1])
        except (IndexError, ValueError):
            pass

        return direction, text

    def _handle_rush_lvl8(self, direction, text):
        agent = self.agent

        if agent.is_leader:
            return

        if agent.level < 2 or (agent.survival_time <= RUSH_MIN_SURVIVAL_TIME and agent.state != State.RALLIED):
            return

        try:
            leader_id = int(text.split("_")[-1])
        except ValueError:
            return

        if agent.target_leader_id is not None and agent.target_leader_id != str(leader_id):
            return

        was_confirmed = agent.leader_confirmed
        agent.target_leader_id = str(leader_id)
        agent.leader_confirmed = True
        if not was_confirmed:
            agent.pending_moves.clear()

        if direction is None:
            return

        agent.rally.on_rush_broadcast(direction)

    def _handle_hello(self, text):
        try:
            heard_id = int(text.split("_")[-1])
        except ValueError:
            return
        self.agent.election.on_hello_broadcast(heard_id)

    def _handle_leader(self, text):
        try:
            leader_id = int(text.split("_")[-1])
        except ValueError:
            return
        self.agent.election.on_leader_broadcast(leader_id)