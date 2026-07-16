##
## EPITECH PROJECT, 2026
## zappy
## File description:
## level_up
##

from AI.algo.constants import LEVEL_UP_REQUIREMENTS, PLAYER_LEVELS_REQUIREMENTS
from AI.algo.inventory import Inventory
from AI.commande.look.look import Look

class LevelUp:
    def __init__(self, inventory, look_cmd):
        self.inventory = inventory
        self.look_cmd = look_cmd
        self.wait_turns = 0
        self.phase = "collect"

    def check_need_level_up(self, level, inventory):
        requirements = LEVEL_UP_REQUIREMENTS.get(level, {})
        for item, required_amount in requirements.items():
            if inventory.data.get(item, 0) < required_amount:
                return False
        return True
    
    def get_target_object(self, level):
        requirements = LEVEL_UP_REQUIREMENTS.get(level, {})
        for item, required_amount in requirements.items():
            if self.inventory.data.get(item, 0) < required_amount:
                return item
        return None

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

    def drop_item(self, plan, inventory, level, my_id="?"):
        requirements = LEVEL_UP_REQUIREMENTS.get(level, {})

        dropped = []
        for item, required_amount in requirements.items():
            for _ in range(required_amount):
                plan.append(f"Set {item}\n")
                dropped.append(item)

        return plan

    def player_requirements_met(self, vision, level):
        current_tile = vision[0][0] if isinstance(vision[0], list) else vision[0]
        if current_tile.count("player") >= 6:
            return True
        return False

    def level_up_move(self, vision, level, my_id, map_width, map_height):
        plan = []

        if level == 1:
            if self.phase == "collect":
                target_object = self.get_target_object(level)
                if target_object:
                    return self.look_cmd.pathfinding_move(vision, target_object, "take", level, map_width, map_height)
                else:
                    self.phase = "drop"

            if self.phase == "drop":
                plan.extend(self.clear_case(vision, my_id))
                plan = self.drop_item(plan, self.inventory, level, my_id)
                self.phase = "incant"
                return plan

            if self.phase == "incant":
                self.phase = "collect"
                return ["Incantation\n"]

            return ["Look\n"]