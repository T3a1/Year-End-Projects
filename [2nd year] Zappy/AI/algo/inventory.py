#
# EPITECH PROJECT, 2026
# Zappy
# File description:
# Inventory management
#

from AI.algo.constants import INVENTORY_TEMPLATE, FOOD_DURATION


class Inventory:

    def __init__(self):
        self.data = INVENTORY_TEMPLATE.copy()

    def parse(self, response):
        inventory = INVENTORY_TEMPLATE.copy()
        response = response.strip()
        response = response.strip("[]")
        items = response.split(",")
        for item in items:
            item = item.strip()
            if " " in item:
                name, quantity = item.split(" ")
                name = name.strip()
                quantity = int(quantity.strip())
                if name in inventory:
                    inventory[name] = quantity
        self.data = inventory
        return inventory

    def update_from_command(self, current_command, survival_time):
        parts = current_command.split()
        if len(parts) > 1 and parts[0] == "Take":
            item = parts[1]
            if item == "food":
                self.data["food"] += 1
                survival_time += FOOD_DURATION
            elif item in self.data:
                self.data[item] += 1
        return survival_time

    def get(self, item, default=0):
        return self.data.get(item, default)
