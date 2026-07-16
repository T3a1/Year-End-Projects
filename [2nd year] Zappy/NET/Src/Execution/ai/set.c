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

static bool resource_in_inventory(ai_t *player, resource_t resource)
{
    if (player->inventory[resource] > 0)
        return true;
    return false;
}

void set(__attribute__((unused)) world_t *world, command_t *command)
{
    resource_t resource = str_to_resource(command->arguments[1]);

    if (resource == (resource_t) -1 ||
        !resource_in_inventory(command->player, resource)) {
        if (world->args->verbose)
            logger_server("Impossible to identify resource %s\n",
                WARNING, command->arguments[1]);
        logger_client(command->player_fd, "ko");
        return;
    }
    world->resource_counts[resource]++;
    command->player->inventory[resource]--;
    command->player->position->content->resources[resource]++;
    send_to_guis(world->gui, "pdr #%zu %d", command->player->id, resource);
    logger_client(command->player_fd, "ok");
}
