/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** disconnect_connection
*/

#include "network.h"
#include "ai.h"

static void remove_conn(network_t *network, connection_t *current,
    connection_t *prev)
{
    connection_t **head = &network->connections;
    connection_t *next = current->next;

    logger_server("Client %s:%d disconnected", INFO,
        inet_ntoa(current->addr.sin_addr), ntohs(current->addr.sin_port));
    if (current->type == AI && current->cli.ai != NULL)
        destroy_ai(network->server->world, &current->cli.ai);
    if (current && current->type == GUI && current->cli.gui != NULL)
        remove_gui(&network->server->world->gui, current->cli.gui);
    if (prev)
        prev->next = next;
    else
        *head = next;
}

bool disconnect_connection(network_t *network, int index)
{
    connection_t *current = network->connections;
    connection_t *prev = NULL;

    while (current) {
        if (current->fd == network->poll_fds[index].fd) {
            remove_conn(network, current, prev);
            close(current->fd);
            free(current->partial_command);
            free(current);
            network->poll_fds[index].fd = -1;
            network->nb_connections--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}
