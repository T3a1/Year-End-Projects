/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** connect_nbr
*/

#include "command.h"
#include "team.h"
#include "map.h"

void connect_nbr(__attribute__((unused)) world_t *world, command_t *command)
{
    logger_client(command->player_fd, "%zu",
        command->player->team->free_slots);
}
