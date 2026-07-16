/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** init_destroy
*/

#include "blacklist.h"

void ban_ip(blacklist_t **head, const struct in_addr *ip)
{
    blacklist_t *new_node = calloc(1, sizeof(blacklist_t));

    if (!new_node)
        return;
    new_node->ban_ip = *ip;
    new_node->next = *head;
    *head = new_node;
}

void destroy_blacklist(blacklist_t *head)
{
    blacklist_t *current = head;
    blacklist_t *next = NULL;

    while (current) {
        next = current->next;
        free(current);
        current = next;
    }
}
