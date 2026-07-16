/*
** EPITECH PROJECT, 2026
** Visual Studio Live Share (Workspace)
** File description:
** handle_unknown
*/

#include "command.h"
#include "ai.h"
#include "utils.h"
#include "parser.h"
#include "team.h"
#include "map.h"
#include "network.h"


static bool handle_graphic_connection(world_t *world,
    connection_t *connection)
{
    graphic(world, connection->cli.gui, NULL, connection->fd);
    connection->type = GUI;
    new_gui(world, connection);
    return true;
}

static void notify_gui_pnw(server_t *server, connection_t *connection)
{
    connection_t *conn = server->network->connections;

    while (conn) {
        if (conn->type == GUI)
            pnw(server->world, connection->cli.ai, conn->fd, NULL);
        conn = conn->next;
    }
}

static bool handle_ai_connection(server_t *server,
    connection_t *connection, char *command)
{
    connection->type = AI;
    connection->cli.ai = bloom_egg(server->world, command);
    if (!connection->cli.ai) {
        logger_server("Failed to create AI for team %s", MAJOR, command);
        return false;
    }
    send_to_guis(server->world->gui, "ebo #%zu", connection->cli.ai->id);
    logger_client(connection->fd, "%zu\n%zu %zu",
        connection->cli.ai->team->free_slots,
        server->args->width, server->args->height);
    notify_gui_pnw(server, connection);
    return true;
}

bool unknown_handler(server_t *server, connection_t *connection,
    char *command)
{
    char **args = split_arguments(command, " ");
    bool success = false;

    if (args == NULL || args[0] == NULL) {
        free_array(args);
        return false;
    }
    if (strcmp(command, "GRAPHIC") == 0)
        success = handle_graphic_connection(server->world, connection);
    else if (is_available_team(server->world, command))
        success = handle_ai_connection(server, connection, command);
    free_array(args);
    return success;
}
