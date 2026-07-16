/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** get_fd_from_connections
*/

#include "connection.h"

int get_fd_from_connections(connection_t *connections, ai_t *ai)
{
    connection_t *conn = connections;

    while (conn) {
        if (conn->cli.ai == ai)
            return conn->fd;
        conn = conn->next;
    }
    return -1;
}
