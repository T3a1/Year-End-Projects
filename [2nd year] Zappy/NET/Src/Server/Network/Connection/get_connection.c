/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** get_connection
*/

#include "connection.h"

connection_t *get_connection(connection_t *head, int fd)
{
    connection_t *current = head;

    while (current) {
        if (current->fd == fd)
            return current;
        current = current->next;
    }
    return NULL;
}
