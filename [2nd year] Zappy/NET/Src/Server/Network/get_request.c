/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** get_request
*/

#include "network.h"

// Maybe change the attribut of the function
char *get_request(network_t *network, int index)
{
    char buffer[BUFFER_SIZE] = {0};
    ssize_t bytes_read = recv(network->poll_fds[index].fd,
        buffer, BUFFER_SIZE - 1, 0);

    if (bytes_read <= 0) {
        disconnect_connection(network, index);
        return NULL;
    }
    buffer[bytes_read] = '\0';
    return strdup(buffer);
}
