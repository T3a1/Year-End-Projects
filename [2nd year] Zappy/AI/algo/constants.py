#
# EPITECH PROJECT, 2026
# Zappy
# File description:
# AI constants
#

OK_COMMANDS = {
    "Forward": 7,
    "Right": 7,
    "Left": 7,
    "Broadcast": 7,
    "Connect_nbr": 0,
    "Fork": 42,
    "Eject": 7,
    "Take": 7,
    "Set": 7,
    "None": 1
}

PAYLOAD_COMMANDS = {
    "Look": 7,
    "Inventory": 1,
    "Incantation": 300,
}

INVENTORY_TEMPLATE = {
    "food": 0,
    "linemate": 0,
    "deraumere": 0,
    "sibur": 0,
    "mendiane": 0,
    "phiras": 0,
    "thystame": 0,
}

PLAYER_LEVELS_REQUIREMENTS = {
    1: 1,
    2: 6,
    3: 6,
    4: 6,
    5: 6,
    6: 6,
    7: 6,
}

LEVEL_UP_REQUIREMENTS = {
    1: {"linemate": 1},
    2: {"linemate": 1, "deraumere": 1, "sibur": 1},
    3: {"linemate": 2, "sibur": 1, "phiras": 2},
    4: {"linemate": 1, "deraumere": 1, "sibur": 2, "phiras": 1},
    5: {"linemate": 1, "deraumere": 2, "sibur": 1, "mendiane": 3},
    6: {"linemate": 1, "deraumere": 2, "sibur": 3, "phiras": 1},
    7: {"linemate": 2, "deraumere": 2, "sibur": 2, "mendiane": 2, "phiras": 2, "thystame": 1},
}

LEVEL_UP_LEADER = {
    "linemate": 8,
    "deraumere": 8,
    "sibur": 10,
    "mendiane": 5,
    "phiras": 6,
    "thystame": 1,
}

FOOD_DURATION = 126
SURVIVAL_TRIGGER = 50

MIN_FOOD_FOR_RALLY = 3000

PLAYER_REQUIRE_WITH_MARGIN = {
    3: 10,
    4: 9,
    5: 8,
    6: 7,
    7: 6,
}
