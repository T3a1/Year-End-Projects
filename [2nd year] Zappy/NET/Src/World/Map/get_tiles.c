/*
** EPITECH PROJECT, 2026
** Visual Studio Live Share (Workspace)
** File description:
** get_tiles
*/

#include "world.h"
#include "map.h"

tile_t *get_random_tile(world_t *world)
{
    size_t x = rand() % world->args->width;
    size_t y = rand() % world->args->height;

    return &world->grid[y * world->args->width + x];
}

tile_t *get_tile(world_t *world, size_t x, size_t y)
{
    size_t width = world->args->width;
    size_t height = world->args->height;
    tile_t *grid = world->grid;

    if (x >= width || y >= height)
        return NULL;
    return &grid[y * world->args->width + x];
}
