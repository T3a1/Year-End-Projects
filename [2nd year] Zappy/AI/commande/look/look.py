#
# EPITECH PROJECT, 2026
# Zappy
# File description:
# Look commande
#

class Look:

    DIRECTION_MOVES = {
        1: ("Forward\n",),
        2: ("Forward\n",),
        3: ("Left\n",),
        4: ("Left\n",),
        5: ("Right\n",),
        6: ("Right\n",),
        7: ("Right\n",),
        8: ("Forward\n",),
    }

    def __init__(self):
        self.turn = 0
        self.turned = 0
        self.sweep_step = 0
        self.sweep_turn = 0

    def parse_look_response(self, response, level):
        response = response.strip()
        response = response.strip("[]")

        raw_tiles = response.split(",")
        vision = []
        current_tile_index = 0
        rows_to_build = level + 1

        for i in range(rows_to_build):
            row_size = (i * 2) + 1
            current_row = []

            for _ in range(row_size):
                if current_tile_index < len(raw_tiles):
                    tile_content = raw_tiles[current_tile_index]
                    objects_in_tile = tile_content.split()
                    current_row.append(objects_in_tile)
                    current_tile_index += 1
                else:
                    current_row.append([])

            vision.append(current_row)

        return vision

    def pathfinding_move(self, vision_or_message, target_object, mode,
                          level, map_width, map_height):
        if mode == "direction":
            return self._direction_move(vision_or_message)

        vision = vision_or_message
        if not vision:
            return ["Look\n"]

        plan = self._move_to_visible_target(vision, target_object, mode)
        if plan is not None:
            return plan

        return self._sweep_move(level, map_width, map_height)

    def _move_to_visible_target(self, vision, target_object, mode):
        target = self._find_closest_target(vision, target_object)
        if target is None:
            return None

        r, c = target
        center = r
        plan = []

        for _ in range(r):
            plan.append("Forward\n")

        if c < center:
            plan.append("Left\n")
            for _ in range(center - c):
                plan.append("Forward\n")
        elif c > center:
            plan.append("Right\n")
            for _ in range(c - center):
                plan.append("Forward\n")

        if mode == "take" and target_object != "player":
            if target_object == "food":
                plan.append("Look\n")
            plan.append(f"Take {target_object}\n")

        return plan

    def _find_closest_target(self, vision, target_object):
        target = None
        min_dist = 9999

        for r, row in enumerate(vision):
            for c, objects_in_tile in enumerate(row):
                if target_object not in objects_in_tile:
                    continue

                if target_object == "player" and r == 0 and c == r:
                    if objects_in_tile.count("player") < 2:
                        continue

                dist = r + abs(c - r)
                if dist < min_dist:
                    min_dist = dist
                    target = (r, c)

        return target

    def _sweep_move(self, level, map_width, map_height):
        plan = []
        vision_range = level + 1
        max_steps_before_turn = max(map_height, map_width)

        if self.sweep_step + vision_range >= max_steps_before_turn:
            remaining = max(max_steps_before_turn - self.sweep_step, 0)
            for _ in range(remaining):
                plan.append("Forward\n")

            self.sweep_turn += 1
            if self.sweep_turn % 2 != 0:
                plan.extend(["Right\n", "Forward\n", "Right\n"])
            else:
                plan.extend(["Left\n", "Forward\n", "Left\n"])

            self.sweep_step = 0
        else:
            half = max(vision_range // 2, 1)
            for _ in range(half):
                plan.append("Forward\n")
            if half < vision_range:
                plan.append("Look\n")
                for _ in range(vision_range - half):
                    plan.append("Forward\n")
            self.sweep_step += vision_range

        plan.append("Look\n")
        return plan

    def _direction_move(self, message):
        if not message:
            return ["Look\n"]

        try:
            direction = int(message.split(",")[0].split()[1])
        except (IndexError, ValueError):
            return ["Look\n"]

        if direction == 0:
            return ["__ARRIVED__"]

        return list(self.DIRECTION_MOVES.get(direction, ("Look\n",)))
