/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** is_banned
*/

#include "blacklist.h"

bool is_banned(blacklist_t *head, const struct in_addr *ip)
{
    blacklist_t *current = head;

    while (current) {
        if (current->ban_ip.s_addr == ip->s_addr)
            return true;
        current = current->next;
    }
    return false;
}
