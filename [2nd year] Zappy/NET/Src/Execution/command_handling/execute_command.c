/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** execute_command
*/

#include "command.h"

static bool execute_refill(server_t *server, command_t *command)
{
    if (strcmp(command->arguments[0], "REFILL") != 0)
        return false;
    spawn_resources(server->world);
    return true;
}

static bool validate_command(command_t *command)
{
    if (command->handler == NULL) {
        logger_server("No handler for command", WARNING);
        return false;
    }
    if (!command->player) {
        logger_server("Command has no player", WARNING);
        return false;
    }
    return true;
}

bool execute_command(server_t *server, command_t *command)
{
    if (command == NULL || (command->player == NULL &&
            strcmp(command->arguments[0], "REFILL") != 0))
        return false;
    if (execute_refill(server, command))
        return true;
    if (!validate_command(command))
        return false;
    command->handler(server->world, command);
    return false;
}
