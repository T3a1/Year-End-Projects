/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** init_destroy
*/

#include "world.h"
#include "resources.h"
#include "ai.h"
#include "command.h"
#include "map.h"

bool alloc_grid_buffers(size_t size, tile_t **grid, world_t **world)
{
    *grid = calloc(size, sizeof(tile_t));
    *world = calloc(1, sizeof(world_t));
    if (!*grid || !*world) {
        free(*grid);
        free(*world);
        return false;
    }
    (*world)->grid = *grid;
    return true;
}

static void set_tile_neighbors(
    tile_t *tile, world_t *world, size_t x, size_t y)
{
    size_t ny = (y - 1 + world->args->height) % world->args->height;
    size_t sy = (y + 1) % world->args->height;
    size_t wx = (x - 1 + world->args->width) % world->args->width;
    size_t ex = (x + 1) % world->args->width;

    tile->north = &world->grid[ny * world->args->width + x];
    tile->south = &world->grid[sy * world->args->width + x];
    tile->west = &world->grid[y * world->args->width + wx];
    tile->east = &world->grid[y * world->args->width + ex];
}

static bool alloc_tile_content(tile_t *tile)
{
    tile->content = calloc(1, sizeof(tile_content_t));
    if (!tile->content)
        return false;
    return true;
}

static bool init_tile(world_t *world, size_t x, size_t y)
{
    size_t idx = y * world->args->width + x;
    tile_t *tile = &world->grid[idx];

    tile->x = x;
    tile->y = y;
    set_tile_neighbors(tile, world, x, y);
    return alloc_tile_content(tile);
}

static bool init_row(world_t *world, size_t y)
{
    for (size_t x = 0; x < world->args->width; x++) {
        if (!init_tile(world, x, y))
            return false;
    }
    return true;
}

bool build_grid(world_t *world)
{
    for (size_t y = 0; y < world->args->height; y++) {
        if (!init_row(world, y))
            return false;
    }
    return true;
}
