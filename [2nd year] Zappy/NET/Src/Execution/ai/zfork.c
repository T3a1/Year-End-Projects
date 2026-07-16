/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** fork
*/

#include "command.h"
#include "team.h"

void zfork(__attribute__((unused)) world_t *world, command_t *command)
{
    ai_t *new_ai =
        spawn_fork_egg(world, command->player->team, command->player);

    if (new_ai == NULL) {
        logger_client(command->player_fd, "ko");
        return;
    }
    command->player->team->free_slots++;
    logger_server("[FORK] Player %zu has spawned a new AI with ID %zu",
        OK, command->player->id, new_ai->id);
    send_to_guis(world->gui, "enw %zu %zu %zu %zu", new_ai->id,
        command->player->id, new_ai->position->x, new_ai->position->y);
    logger_client(command->player_fd, "ok");
}
