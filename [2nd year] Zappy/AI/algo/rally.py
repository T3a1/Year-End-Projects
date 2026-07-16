#
# EPITECH PROJECT, 2026
# Zappy
# File description:
# Rally logic (following the leader to the incantation tile)
#

from AI.algo.state import State

RALLY_ABORT_SURVIVAL_TIME = 0

DIRECTION_TO_MOVE = {
    1: "Forward\n",
    2: "Forward\n",
    8: "Forward\n",
    3: "Left\n",
    4: "Left\n",
    5: "Right\n",
    6: "Right\n",
    7: "Right\n",
}


class RallyManager:

    def __init__(self, agent):
        self.agent = agent

    def on_rush_broadcast(self, direction):
        agent = self.agent

        if direction == 0:
            agent.last_rally_direction = None
            agent.state = State.ARRIVED
            return

        agent.last_rally_direction = direction
        agent.state = State.RALLIED

    def next_move(self):
        agent = self.agent

        if agent.state == State.ARRIVED:
            return self._arrived_move()

        return self._rallied_move()

    def _arrived_move(self):
        return "Inventory\n"

    def _rallied_move(self):
        agent = self.agent

        if agent.survival_time < RALLY_ABORT_SURVIVAL_TIME:
            agent.last_rally_direction = None
            agent.state = State.DEFAULT
            return agent.survival.next_move()

        direction = agent.last_rally_direction
        if direction is None:
            return "Inventory\n"

        move = DIRECTION_TO_MOVE.get(direction, "Look\n")
        agent.last_rally_direction = None
        return move
