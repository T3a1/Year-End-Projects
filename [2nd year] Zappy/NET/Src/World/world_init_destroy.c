/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** init
*/

#include "world.h"
#include "resources.h"
#include "ai.h"
#include "gui.h"
#include "command.h"
#include "team.h"
#include "map.h"
#include "utils.h"

static void free_tile_content(tile_t *tile)
{
    player_t *player = NULL;

    if (tile->content) {
        for (player_t *tmp = tile->content->players; tmp;) {
            player = tmp;
            tmp = tmp->next;
            free(player);
        }
        free(tile->content);
        tile->content = NULL;
    }
}

static void free_pending_commands(world_t *world)
{
    command_t *current = world->command_queue;
    command_t *next;

    while (current) {
        next = current->next;
        free_array(current->arguments);
        free(current);
        current = next;
    }
}

void destroy_world(world_t *world)
{
    if (!world)
        return;
    free_teams(world->teams);
    for (size_t i = 0; i < world->args->width * world->args->height; i++) {
        free_tile_content(&world->grid[i]);
    }
    free_pending_commands(world);
    destroy_gui(world->gui);
    destroy_ais(world->ais);
    free(world->grid);
    free(world);
}

static bool create_refill_command(world_t *world)
{
    char **args = calloc(2, sizeof(char *));
    command_t *cmd = NULL;

    if (!args)
        return false;
    args[0] = strdup("REFILL");
    if (!args[0]) {
        free(args);
        return false;
    }
    args[1] = NULL;
    cmd = create_command(args, NULL, 20000 / world->args->freq, -1);
    if (!cmd) {
        free_array(args);
        return false;
    }
    add_global_command(world, cmd);
    return true;
}

static void init_world_fields(world_t *world, tile_t *grid)
{
    world->grid = grid;
    world->ais = NULL;
    world->gui = NULL;
    world->teams = NULL;
    world->next_id = 0;
    world->old_time_commands = 0;
    world->next_command_time = -1;
    memset(world->resource_counts, 0, sizeof(world->resource_counts));
}

static bool setup_world(world_t *world, tile_t *grid)
{
    init_world_fields(world, grid);
    if (!create_refill_command(world))
        return false;
    if (!spawn_resources(world))
        return false;
    if (!init_teams(world))
        return false;
    return spawn_eggs(world);
}

world_t *create_world(args_t *args)
{
    tile_t *grid = NULL;
    world_t *world = NULL;
    size_t size = args->width * args->height;

    if (args->width < 10 || args->height < 10)
        return NULL;
    if (!alloc_grid_buffers(size, &grid, &world))
        return NULL;
    world->args = args;
    if (!build_grid(world)) {
        free(grid);
        free(world);
        return NULL;
    }
    if (!setup_world(world, grid)) {
        free(grid);
        free(world);
        return NULL;
    }
    return world;
}
