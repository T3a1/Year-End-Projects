/*
** EPITECH PROJECT, 2026
** Visual Studio Live Share (Workspace)
** File description:
** handle_gui_commands
*/

#include "gui.h"
#include "command.h"
#include "gui_commands.h"
#include "utils.h"

static const command_gui_t *find_gui_command(char *name)
{
    for (int i = 0; commands_gui[i].command != NULL; i++) {
        if (strcmp(name, commands_gui[i].command) == 0)
            return &commands_gui[i];
    }
    return NULL;
}

static bool execute_gui_command(world_t *world, client_data_t data,
    char **args, int fd)
{
    const command_gui_t *cmd = find_gui_command(args[0]);
    size_t nb_args = (size_t) count_args(args);

    if (!cmd) {
        logger_server("Unknown GUI command", WARNING);
        logger_client(fd, "suc");
        return false;
    }
    if (nb_args < cmd->min_arg || nb_args > cmd->max_arg) {
        logger_server("Invalid number of arguments for GUI command", WARNING);
        logger_client(fd, "sbp");
        return false;
    }
    cmd->handler(world, data.gui, args, fd);
    return true;
}

bool gui_handler(world_t *world, client_data_t data, char *command, int fd)
{
    char **args = NULL;
    bool success = false;

    if (strlen(command) == 0)
        return false;
    args = split_arguments(command, " ");
    if (data.gui == NULL || args == NULL || args[0] == NULL) {
        free_array(args);
        return false;
    }
    success = execute_gui_command(world, data, args, fd);
    free_array(args);
    return success;
}
