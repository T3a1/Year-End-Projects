/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** take
*/

#include "command.h"
#include "map.h"
#include "ai.h"
#include "resources.h"
#include "utils.h"

resource_t str_to_resource(char *str)
{
    const char *RESOURCES_STR[] = {
        "food",
        "linemate",
        "deraumere",
        "sibur",
        "mendiane",
        "phiras",
        "thystame"
    };

    for (int i = 0; i < RESOURCES_COUNT; i++) {
        if (strcmp(RESOURCES_STR[i], str) == 0)
            return RESOURCES[i];
    }
    return -1;
}

static bool resource_on_tile(tile_t *tile, resource_t resource)
{
    if (tile->content->resources[resource] > 0) {
        tile->content->resources[resource]--;
        return true;
    }
    return false;
}

void take(__attribute__((unused)) world_t *world, command_t *command)
{
    resource_t resource = str_to_resource(command->arguments[1]);

    if (resource == (resource_t) -1 ||
        !resource_on_tile(command->player->position, resource)) {
        if (world->args->verbose) {
            logger_server("Impossible to identify resource %s\n",
                WARNING, command->arguments[1]);
        }
        logger_client(command->player_fd, "ko");
        return;
    }
    command->player->inventory[resource]++;
    world->resource_counts[resource]--;
    send_to_guis(world->gui, "pgt #%zu %d", command->player->id, resource);
    logger_client(command->player_fd, "ok");
}
