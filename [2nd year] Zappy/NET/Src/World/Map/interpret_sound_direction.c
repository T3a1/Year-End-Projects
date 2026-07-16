/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** interpret_sound_direction
*/

#include "ai.h"
#include "world.h"
#include "map.h"

static int get_circle_index(absolute_direction_t dir)
{
    switch (dir) {
        case ABS_NORTH:
            return 0;
        case ABS_NORTH_WEST:
            return 1;
        case ABS_WEST:
            return 2;
        case ABS_SOUTH_WEST:
            return 3;
        case ABS_SOUTH:
            return 4;
        case ABS_SOUTH_EAST:
            return 5;
        case ABS_EAST:
            return 6;
        case ABS_NORTH_EAST:
            return 7;
        default:
            return -1;
    }
}

static int get_facing_circle_index(direction_t facing)
{
    switch (facing) {
        case NORTH:
            return 0;
        case SOUTH:
            return 4;
        case EAST:
            return 6;
        case WEST:
            return 2;
        default:
            return -1;
    }
}

static int get_digit_from_direction
(direction_t facing, absolute_direction_t target)
{
    int f = get_facing_circle_index(facing);
    int t = get_circle_index(target);

    if (f == -1 || t == -1)
        return -1;
    return ((t - f + 8) % 8) + 1;
}

static int vec_to_index(int component)
{
    if (component > 0)
        return 0;
    if (component < 0)
        return 2;
    return 1;
}

static absolute_direction_t vector_to_origin(int_pair_t *v)
{
    static const absolute_direction_t table[3][3] = {
        { ABS_NORTH_WEST, ABS_NORTH, ABS_NORTH_EAST },
        { ABS_WEST, -1, ABS_EAST },
        { ABS_SOUTH_WEST, ABS_SOUTH, ABS_SOUTH_EAST }
    };

    return table[vec_to_index(v->b)][vec_to_index(v->a)];
}

int get_dir_from_vector(int_pair_t *v, direction_t facing)
{
    absolute_direction_t origin;

    if (v->a == 0 && v->b == 0)
        return 0;
    origin = vector_to_origin(v);
    return get_digit_from_direction(facing, origin);
}
