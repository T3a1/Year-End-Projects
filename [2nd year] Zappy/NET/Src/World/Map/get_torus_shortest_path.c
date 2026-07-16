/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** get_torus_shortest_path
*/

#include "world.h"
#include "map.h"

static int_pair_t calc_axis_diff(int start, int target, int size)
{
    int_pair_t res = {0, 0};

    res.a = target - start;
    if (res.a > 0)
        res.b = res.a - size;
    else
        res.b = res.a + size;
    return res;
}

static int pick_shorter(int opt_a, int opt_b)
{
    int abs_a = (opt_a < 0) ? -opt_a : opt_a;
    int abs_b = (opt_b < 0) ? -opt_b : opt_b;

    return (abs_b < abs_a) ? opt_b : opt_a;
}

static int get_axis_vec(size_t s, size_t t,
    int max_val)
{
    int_pair_t diffs = calc_axis_diff((int)s, (int)t, max_val);

    return pick_shorter(diffs.a, diffs.b);
}

int_pair_t torus_shortest_vector(world_t *world,
    const pos2i_t *start, const pos2i_t *end)
{
    int w = (int)world->args->width;
    int h = (int)world->args->height;
    int_pair_t result;

    result.a = get_axis_vec(start->x, end->x, w);
    result.b = get_axis_vec(start->y, end->y, h);
    return result;
}
