/*
** EPITECH PROJECT, 2026
** Visual Studio Live Share (Workspace)
** File description:
** handle_ai_commands
*/

#include "command.h"
#include "ai.h"
#include "ai_commands.h"
#include "utils.h"
#include "parser.h"

void (*get_handler(char **command))
(world_t *world, command_t *command)
{
    for (size_t i = 0; i < NB_COMMANDS_AI; i++) {
        if (strcmp(command[0], commands_ai[i].command) == 0)
            return commands_ai[i].handler;
    }
    return NULL;
}

static const command_ai_t *find_ai_command_by_name(const char *name)
{
    for (size_t i = 0; i < NB_COMMANDS_AI; i++) {
        if (strcmp(name, commands_ai[i].command) == 0)
            return &commands_ai[i];
    }
    return NULL;
}

static bool validate_ai_args(const command_ai_t *cmd, size_t nb_args)
{
    if (!cmd)
        return false;
    if (nb_args >= cmd->min_arg && nb_args <= cmd->max_arg)
        return true;
    logger_server("Invalid number of arguments for AI command", WARNING);
    return false;
}

static bool enqueue_ai_command(world_t *world, client_data_t data,
    char **args, int fd)
{
    const command_ai_t *cmd = find_ai_command_by_name(args[0]);
    command_t *new_command = NULL;
    long duration = 0;

    if (!cmd) {
        logger_server("Unknown AI command", WARNING);
        return false;
    }
    if (!validate_ai_args(cmd, (size_t)count_args(args)))
        return false;
    duration = (cmd->time_unit * 1000) / world->args->freq;
    new_command = create_command(args, data.ai, duration, fd);
    if (!new_command)
        return false;
    add_command(world, new_command);
    return true;
}

bool ai_handler(world_t *world, client_data_t data, char *command, int fd)
{
    char **args = NULL;
    bool success = false;

    if (strlen(command) == 0)
        return false;
    args = split_arguments(command, " ");
    if (data.ai == NULL || args == NULL || args[0] == NULL) {
        free_array(args);
        return false;
    }
    success = enqueue_ai_command(world, data, args, fd);
    if (!success) {
        free_array(args);
        return false;
    }
    return success;
}
