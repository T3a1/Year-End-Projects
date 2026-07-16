/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Unit tests for map / navigation functions
*/

#include <criterion/criterion.h>
#include <stdlib.h>
#include <string.h>

#include "world.h"
#include "map.h"
#include "args.h"
#include "command.h"

/*
** ─── HELPERS ─────────────────────────────────────────────────────────────────
*/

static args_t *make_args(size_t w, size_t h)
{
    args_t *a = calloc(1, sizeof(args_t));

    a->width = w;
    a->height = h;
    a->freq = 1;
    a->eggs_per_team = 1;
    a->team_names = malloc(sizeof(char *) * 2);
    a->team_names[0] = strdup("T1");
    a->team_names[1] = NULL;
    a->parsing_status = true;
    return a;
}

static world_t *make_world(size_t w, size_t h)
{
    args_t *args = make_args(w, h);
    tile_t *grid = NULL;
    world_t *world = NULL;

    alloc_grid_buffers(w * h, &grid, &world);
    world->args = args;
    build_grid(world);
    return world;
}

static void free_world_shallow(world_t *world)
{
    size_t n = world->args->width * world->args->height;

    for (size_t i = 0; i < n; i++)
        free(world->grid[i].content);
    free(world->grid);
    free(world->args->team_names[0]);
    free(world->args->team_names);
    free(world->args);
    free(world);
}

/*
** ─── torus_shortest_vector ───────────────────────────────────────────────────
*/

Test(torus_shortest_vector, same_position_is_zero)
{
    world_t *world = make_world(10, 10);
    pos2i_t p = {3, 4};
    int_pair_t v = torus_shortest_vector(world, &p, &p);

    cr_assert_eq(v.a, 0);
    cr_assert_eq(v.b, 0);
    free_world_shallow(world);
}

Test(torus_shortest_vector, direct_east_offset)
{
    world_t *world = make_world(10, 10);
    pos2i_t s = {0, 0};
    pos2i_t e = {3, 0};
    int_pair_t v = torus_shortest_vector(world, &s, &e);

    cr_assert_eq(v.a, 3);
    cr_assert_eq(v.b, 0);
    free_world_shallow(world);
}

Test(torus_shortest_vector, wraps_shorter_west)
{
    /* On a 10-wide map, going from x=1 to x=8 direct = +7,
       but wrapping west = -3 which is shorter */
    world_t *world = make_world(10, 10);
    pos2i_t s = {1, 0};
    pos2i_t e = {8, 0};
    int_pair_t v = torus_shortest_vector(world, &s, &e);

    cr_assert_eq(v.a, -3);
    free_world_shallow(world);
}

Test(torus_shortest_vector, direct_south_offset)
{
    world_t *world = make_world(10, 10);
    pos2i_t s = {0, 0};
    pos2i_t e = {0, 2};
    int_pair_t v = torus_shortest_vector(world, &s, &e);

    cr_assert_eq(v.b, 2);
    cr_assert_eq(v.a, 0);
    free_world_shallow(world);
}

Test(torus_shortest_vector, diagonal_direct)
{
    world_t *world = make_world(10, 10);
    pos2i_t s = {0, 0};
    pos2i_t e = {2, 3};
    int_pair_t v = torus_shortest_vector(world, &s, &e);

    cr_assert_eq(v.a, 2);
    cr_assert_eq(v.b, 3);
    free_world_shallow(world);
}

Test(torus_shortest_vector, wraps_around_corner)
{
    /* 10x10: s=(9,9) e=(0,0) → shortest is (-1,-1) not (9,9) */
    world_t *world = make_world(10, 10);
    pos2i_t s = {9, 9};
    pos2i_t e = {0, 0};
    int_pair_t v = torus_shortest_vector(world, &s, &e);

    /* absolute offset ≤ half width/height */
    cr_assert_leq(v.a * v.a, 1);
    cr_assert_leq(v.b * v.b, 1);
    free_world_shallow(world);
}

/*
** ─── get_dir_from_vector ─────────────────────────────────────────────────────
*/

Test(get_dir_from_vector, zero_vector_returns_zero)
{
    int_pair_t v = {0, 0};

    cr_assert_eq(get_dir_from_vector(&v, NORTH), 0);
}

Test(get_dir_from_vector, north_facing_north_source_is_1)
{
    int_pair_t v = {0, 1};

    cr_assert_eq(get_dir_from_vector(&v, NORTH), 1);
}


Test(get_dir_from_vector, east_facing_north_is_3)
{
    int_pair_t v = {1, 0};

    cr_assert_eq(get_dir_from_vector(&v, NORTH), 3);
}

Test(get_dir_from_vector, south_facing_north_is_5)
{
    int_pair_t v = {0, -1};

    cr_assert_eq(get_dir_from_vector(&v, NORTH), 5);
}

Test(get_dir_from_vector, west_facing_north_is_7)
{
    int_pair_t v = {-1, 0};

    cr_assert_eq(get_dir_from_vector(&v, NORTH), 7);
}

/*
** ─── get_tile ────────────────────────────────────────────────────────────────
*/

Test(get_tile, returns_correct_tile)
{
    world_t *world = make_world(10, 10);
    tile_t *t = get_tile(world, 3, 4);

    cr_assert_not_null(t);
    cr_assert_eq(t->x, 3);
    cr_assert_eq(t->y, 4);
    free_world_shallow(world);
}

Test(get_tile, returns_null_out_of_bounds_x)
{
    world_t *world = make_world(10, 10);

    cr_assert_null(get_tile(world, 10, 0));
    free_world_shallow(world);
}

Test(get_tile, returns_null_out_of_bounds_y)
{
    world_t *world = make_world(10, 10);

    cr_assert_null(get_tile(world, 0, 10));
    free_world_shallow(world);
}

Test(get_tile, returns_origin_tile)
{
    world_t *world = make_world(10, 10);
    tile_t *t = get_tile(world, 0, 0);

    cr_assert_not_null(t);
    cr_assert_eq(t->x, 0);
    cr_assert_eq(t->y, 0);
    free_world_shallow(world);
}

/*
** ─── get_front / get_right / get_left ───────────────────────────────────────
*/

Test(get_front, north_returns_north_tile)
{
    world_t *world = make_world(10, 10);
    tile_t *t = get_tile(world, 5, 5);

    cr_assert_eq(get_front(t, NORTH), t->north);
    free_world_shallow(world);
}

Test(get_front, south_returns_south_tile)
{
    world_t *world = make_world(10, 10);
    tile_t *t = get_tile(world, 5, 5);

    cr_assert_eq(get_front(t, SOUTH), t->south);
    free_world_shallow(world);
}

Test(get_front, east_returns_east_tile)
{
    world_t *world = make_world(10, 10);
    tile_t *t = get_tile(world, 5, 5);

    cr_assert_eq(get_front(t, EAST), t->east);
    free_world_shallow(world);
}

Test(get_front, west_returns_west_tile)
{
    world_t *world = make_world(10, 10);
    tile_t *t = get_tile(world, 5, 5);

    cr_assert_eq(get_front(t, WEST), t->west);
    free_world_shallow(world);
}

Test(get_right, north_facing_right_is_east)
{
    world_t *world = make_world(10, 10);
    tile_t *t = get_tile(world, 5, 5);

    cr_assert_eq(get_right(t, NORTH), t->east);
    free_world_shallow(world);
}

Test(get_right, east_facing_right_is_south)
{
    world_t *world = make_world(10, 10);
    tile_t *t = get_tile(world, 5, 5);

    cr_assert_eq(get_right(t, EAST), t->south);
    free_world_shallow(world);
}

Test(get_right, south_facing_right_is_west)
{
    world_t *world = make_world(10, 10);
    tile_t *t = get_tile(world, 5, 5);

    cr_assert_eq(get_right(t, SOUTH), t->west);
    free_world_shallow(world);
}

Test(get_right, west_facing_right_is_north)
{
    world_t *world = make_world(10, 10);
    tile_t *t = get_tile(world, 5, 5);

    cr_assert_eq(get_right(t, WEST), t->north);
    free_world_shallow(world);
}

Test(get_left, north_facing_left_is_west)
{
    world_t *world = make_world(10, 10);
    tile_t *t = get_tile(world, 5, 5);

    cr_assert_eq(get_left(t, NORTH), t->west);
    free_world_shallow(world);
}

Test(get_left, east_facing_left_is_north)
{
    world_t *world = make_world(10, 10);
    tile_t *t = get_tile(world, 5, 5);

    cr_assert_eq(get_left(t, EAST), t->north);
    free_world_shallow(world);
}

Test(get_left, south_facing_left_is_east)
{
    world_t *world = make_world(10, 10);
    tile_t *t = get_tile(world, 5, 5);

    cr_assert_eq(get_left(t, SOUTH), t->east);
    free_world_shallow(world);
}

Test(get_left, west_facing_left_is_south)
{
    world_t *world = make_world(10, 10);
    tile_t *t = get_tile(world, 5, 5);

    cr_assert_eq(get_left(t, WEST), t->south);
    free_world_shallow(world);
}

/*
** ─── torus wrap-around neighbours ───────────────────────────────────────────
*/

Test(map_torus, north_of_row0_wraps_to_last_row)
{
    world_t *world = make_world(10, 10);
    tile_t *t = get_tile(world, 5, 0);

    cr_assert_eq(t->north->y, (int)(world->args->height - 1));
    free_world_shallow(world);
}

Test(map_torus, south_of_last_row_wraps_to_row0)
{
    world_t *world = make_world(10, 10);
    tile_t *t = get_tile(world, 5, 9);

    cr_assert_eq(t->south->y, 0);
    free_world_shallow(world);
}

Test(map_torus, east_of_last_col_wraps_to_col0)
{
    world_t *world = make_world(10, 10);
    tile_t *t = get_tile(world, 9, 5);

    cr_assert_eq(t->east->x, 0);
    free_world_shallow(world);
}

Test(map_torus, west_of_col0_wraps_to_last_col)
{
    world_t *world = make_world(10, 10);
    tile_t *t = get_tile(world, 0, 5);

    cr_assert_eq(t->west->x, (int)(world->args->width - 1));
    free_world_shallow(world);
}
