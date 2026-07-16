/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** init_destroy
*/

#include "connection.h"
#include "logger.h"

static connection_t *create_connection(int fd, const struct sockaddr_in *addr)
{
    connection_t *new_conn = calloc(1, sizeof(connection_t));

    if (!new_conn)
        return NULL;
    new_conn->fd = fd;
    new_conn->addr = *addr;
    new_conn->type = UNKNOWN;
    new_conn->cli.ai = NULL;
    new_conn->cli.gui = NULL;
    new_conn->partial_command = NULL;
    new_conn->partial_len = 0;
    new_conn->next = NULL;
    return new_conn;
}

bool add_connection(network_t *network, int fd,
    const struct sockaddr_in *addr)
{
    connection_t *new_conn = create_connection(fd, addr);
    connection_t *current = network->connections;

    if (!new_conn)
        return false;
    for (size_t i = 0; i < network->max_connections + 1; i++) {
        if (network->poll_fds[i].fd == -1) {
            network->poll_fds[i].fd = fd;
            network->poll_fds[i].events = POLLIN;
            break;
        }
    }
    if (!network->connections) {
        network->connections = new_conn;
        return true;
    }
    while (current->next)
        current = current->next;
    current->next = new_conn;
    return true;
}

void destroy_connections(connection_t *head)
{
    connection_t *current = head;
    connection_t *next = NULL;

    while (current) {
        next = current->next;
        if (current->partial_command)
            free(current->partial_command);
        free(current);
        current = next;
    }
}
