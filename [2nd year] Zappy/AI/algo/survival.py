#
# EPITECH PROJECT, 2026
# Zappy
# File description:
# Survival behavior (food gathering when not leader / not rallied)
#

from AI.algo.state import State

SURVIVAL_LOW_THRESHOLD = 3000
SURVIVAL_RECOVERED_THRESHOLD = 3700
LEVEL_2_FOOD_CAP = 4600


class SurvivalManager:


    def __init__(self, agent):
        self.agent = agent

    def update_state(self):
        """Met a jour l'etat SURVIVAL/DEFAULT selon le niveau de survie.
        N'affecte jamais ARRIVED, RALLIED ou LEADER : ces etats sont geres
        ailleurs et ne doivent pas etre ecrases ici."""
        agent = self.agent

        if agent.state in (State.ARRIVED, State.RALLIED, State.LEADER):
            return

        if agent.survival_time < SURVIVAL_LOW_THRESHOLD:
            agent.state = State.SURVIVAL
        elif agent.survival_time > SURVIVAL_RECOVERED_THRESHOLD:
            if agent.state == State.SURVIVAL:
                agent.state = State.DEFAULT

    def next_move(self):
        agent = self.agent

        if agent.level == 2 and agent.survival_time >= LEVEL_2_FOOD_CAP:
            return "Look\n"

        if not agent.vision:
            return "Look\n"

        current_vision = agent.vision
        agent.vision = []

        plan = agent.look_cmd.pathfinding_move(
            current_vision, "food", "take", agent.level,
            agent.map_width, agent.map_height,
        )
        if plan:
            agent.pending_moves.extend(plan[1:])
            return plan[0]

        return "Look\n"
