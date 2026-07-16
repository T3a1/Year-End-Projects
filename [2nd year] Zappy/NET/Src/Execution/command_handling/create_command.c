/*
** EPITECH PROJECT, 2026
** Visual Studio Live Share (Workspace)
** File description:
** create_command
*/

#include "command.h"

static void fill_command(command_t *cmd, char **command,
    ai_t *player, time_t remaining_time)
{
    cmd->arguments = command;
    cmd->player = player;
    cmd->remaining_time = remaining_time;
    cmd->next = NULL;
    cmd->handler = NULL;
    if (strcmp(command[0], "REFILL") != 0) {
        cmd->handler = get_handler(command);
    }
}

command_t *create_command(char **command, ai_t *player,
    time_t remaining_time, int player_fd)
{
    command_t *cmd = calloc(1, sizeof(command_t));

    if (!cmd)
        return NULL;
    if (!player && strcmp(command[0], "REFILL") != 0) {
        free(cmd);
        return NULL;
    }
    if (player && player->is_incanting) {
        free(cmd);
        return NULL;
    }
    fill_command(cmd, command, player, remaining_time);
    cmd->player_fd = player_fd;
    return cmd;
}
