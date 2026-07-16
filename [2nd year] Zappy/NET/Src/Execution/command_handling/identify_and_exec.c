/*
** EPITECH PROJECT, 2026
** Visual Studio Live Share (Workspace)
** File description:
** identify_command
*/

#include "command.h"
#include "server.h"
#include "logger.h"

static bool send_to_handler
(server_t *server, connection_t *conn, char *command)
{
    switch (conn->type) {
        case GUI:
            return gui_handler(server->world, conn->cli, command, conn->fd);
        case AI:
            return ai_handler(server->world, conn->cli, command, conn->fd);
        default:
            if (conn->type != UNKNOWN) {
                logger_client(conn->fd, "You're not possible");
                server->exit_status = EXIT_FAILURE;
                return false;
            }
            return unknown_handler(server, conn, command);
    }
}

static void handle_firewall(server_t *server, connection_t *conn,
    unsigned int client_id)
{
    logger_client(conn->fd, "You have been banned");
    logger_server("Client %s:%d has been banned", WARNING,
        inet_ntoa(conn->addr.sin_addr), ntohs(conn->addr.sin_port));
    ban_ip(&server->network->blacklist, &conn->addr.sin_addr);
    disconnect_connection(server->network, client_id);
}

void execute_commands(server_t *server, char **commands,
    unsigned int client_id)
{
    const int curr_fd = server->network->poll_fds[client_id].fd;
    connection_t *conn = get_connection(server->network->connections, curr_fd);

    if (!commands)
        return;
    for (size_t i = 0; commands[i] != NULL; i++) {
        if (send_to_handler(server, conn, commands[i]))
            continue;
        if (conn->type == AI)
            logger_client(conn->fd, "ko");
        if (server->args->firewall)
            handle_firewall(server, conn, client_id);
    }
}
