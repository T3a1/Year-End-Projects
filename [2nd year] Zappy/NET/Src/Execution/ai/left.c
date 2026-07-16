/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** forward
*/

#include "command.h"

void left(__attribute__((unused)) world_t *world, command_t *command)
{
    switch (command->player->direction) {
        case NORTH:
            command->player->direction = WEST;
            break;
        case SOUTH:
            command->player->direction = EAST;
            break;
        case EAST:
            command->player->direction = NORTH;
            break;
        case WEST:
            command->player->direction = SOUTH;
            break;
    }
    logger_client(command->player_fd, "ok");
}
