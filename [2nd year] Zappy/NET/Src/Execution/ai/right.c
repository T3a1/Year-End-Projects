/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** right
*/

#include "command.h"

void right(__attribute__((unused)) world_t *world, command_t *command)
{
    switch (command->player->direction) {
        case NORTH:
            command->player->direction = EAST;
            break;
        case SOUTH:
            command->player->direction = WEST;
            break;
        case EAST:
            command->player->direction = SOUTH;
            break;
        case WEST:
            command->player->direction = NORTH;
            break;
    }
    logger_client(command->player_fd, "ok");
}
