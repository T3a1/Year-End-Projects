#
# EPITECH PROJECT, 2026
# Zappy
# File description:
# Leader behavior
#

from AI.algo.constants import LEVEL_UP_LEADER, LEVEL_UP_REQUIREMENTS

LEADER_HEARTBEAT_INTERVAL = 50
LEADER_RECALL_INTERVAL = 2
MIN_PLAYERS_LEVEL_UP_3 = 6
FOOD_SAFE_THRESHOLD = 4300
STABILIZATION_TURNS_REQUIRED = 5
FIRST_RITUAL_EXTRA_WAIT = 3


class LeaderManager:

    def __init__(self, agent):
        self.agent = agent
        self.is_first_ritual = True 

    def has_all_leader_items(self):
        agent = self.agent
        for item, required_qty in LEVEL_UP_LEADER.items():
            if agent.inventory.data.get(item, 0) < required_qty:
                return False
        return True

    def get_missing_leader_items(self):
        agent = self.agent
        missing = []
        for item, required_qty in LEVEL_UP_LEADER.items():
            if agent.inventory.data.get(item, 0) < required_qty:
                missing.append(item)
        return missing

    def get_missing_leader_stone(self):
        agent = self.agent
        for item, required_qty in LEVEL_UP_LEADER.items():
            if agent.inventory.data.get(item, 0) < required_qty:
                return item
        return "food"

    def players_on_my_tile(self):
        agent = self.agent
        if not agent.vision:
            return 1
        current_tile = agent.vision[0][0] if isinstance(agent.vision[0], list) else agent.vision[0]
        return current_tile.count("player")

    def next_move(self):
        agent = self.agent

        agent.turns_since_heartbeat += 1
        if agent.turns_since_heartbeat >= LEADER_HEARTBEAT_INTERVAL:
            agent.turns_since_heartbeat = 0
            return agent.make_broadcast(f"Leader_{agent.id}")

        if not agent.ready_for_ritual:
            move = self._farm_phase()
            if move is not None:
                return move

        return self._ritual_phase()

    def _farm_phase(self):
        agent = self.agent

        if not self.has_all_leader_items():
            target_stone = self.get_missing_leader_stone()
            return agent.farm_target(target_stone)

        if not agent.farming_food:
            if agent.survival_time >= FOOD_SAFE_THRESHOLD:
                agent.farming_food = True
            else:
                return agent.farm_target("food")

        if agent.survival_time < FOOD_SAFE_THRESHOLD:
            return agent.farm_target("food")

        agent.ready_for_ritual = True
        agent.pending_moves.clear()
        return None

    def _ritual_phase(self):
        agent = self.agent

        if not agent.leader_stopped:
            agent.leader_stopped = True
            agent.turns_since_last_call = 0
            return agent.make_broadcast(f"RushLvl8_{agent.id}")

        nb_players = self.players_on_my_tile()

        if nb_players < MIN_PLAYERS_LEVEL_UP_3:
            agent.turns_since_last_call += 1
            agent.players_confirmed_turns = 0
            if agent.turns_since_last_call >= LEADER_RECALL_INTERVAL:
                agent.turns_since_last_call = 0
                return agent.make_broadcast(f"RushLvl8_{agent.id}")
            return "Look\n"

        agent.players_confirmed_turns += 1
        
        target_stabilization = STABILIZATION_TURNS_REQUIRED
        if self.is_first_ritual:
            target_stabilization += FIRST_RITUAL_EXTRA_WAIT

        if agent.players_confirmed_turns < target_stabilization:
            agent.vision = []
            return "Look\n"

        agent.players_confirmed_turns = 0
        self.is_first_ritual = False 
        return self._start_incantation()

    def clear_case(self, vision, my_id="?"):
        plan = []
        current_tile = vision[0][0] if isinstance(vision[0], list) else vision[0]
        if isinstance(current_tile, str):
            items = current_tile.split()
        else:
            items = current_tile

        for item in items:
            if item and item not in ["player", "food"]:
                plan.append(f"Take {item}\n")

        return plan

    def _start_incantation(self):
        agent = self.agent

        if agent.level >= 8:
            return None

        plan = []
        plan.extend(self.clear_case(agent.vision, agent.id))

        requirements = LEVEL_UP_REQUIREMENTS.get(agent.level, {})
        for item, qty in requirements.items():
            for _ in range(qty):
                plan.append(f"Set {item}\n")

        plan.append("Incantation\n")

        agent.pending_moves.extend(plan[1:])
        return plan[0]