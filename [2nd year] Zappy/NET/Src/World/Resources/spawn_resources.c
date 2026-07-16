/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** spawn_resources
*/

#include "world.h"
#include "resources.h"
#include "map.h"

static int get_resource_index(resource_t resource)
{
    for (int i = 0; i < RESOURCES_COUNT; i++) {
        if (RESOURCES[i] == resource)
            return i;
    }
    return -1;
}

static void place_resource(tile_t *tile, resource_t resource)
{
    int index = get_resource_index(resource);

    if (index == -1)
        return;
    tile->content->resources[index]++;
}

static void shuffle_tiles(tile_t **tiles, size_t count)
{
    size_t j = 0;
    tile_t *tmp = NULL;

    for (size_t i = count - 1; i > 0; i--) {
        j = rand() % (i + 1);
        tmp = tiles[i];
        tiles[i] = tiles[j];
        tiles[j] = tmp;
    }
}

static tile_t **build_tile_array(world_t *world, size_t *out_count)
{
    size_t width = world->args->width;
    size_t height = world->args->height;
    size_t count = width * height;
    tile_t **tiles = malloc(count * sizeof(tile_t *));

    if (!tiles)
        return NULL;
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++)
            tiles[i * width + j] = &world->grid[i * width + j];
    }
    *out_count = count;
    return tiles;
}

static void spawn_one_resource(world_t *world, tile_t **tiles,
    size_t count, size_t i)
{
    size_t target = (size_t)(world->args->width
        * world->args->height * RESOURCES_DENSITIES[i]);
    size_t missing = 0;

    shuffle_tiles(tiles, count);
    if (world->resource_counts[i] >= target)
        return;
    missing = target - world->resource_counts[i];
    for (size_t j = 0; j < missing; j++) {
        place_resource(tiles[j % count], RESOURCES[i]);
        world->resource_counts[i]++;
    }
}

bool spawn_resources(world_t *world)
{
    size_t count = 0;
    tile_t **tiles = build_tile_array(world, &count);

    if (!tiles)
        return false;
    for (size_t i = 0; i < RESOURCES_COUNT; i++)
        spawn_one_resource(world, tiles, count, i);
    free(tiles);
    return true;
}
