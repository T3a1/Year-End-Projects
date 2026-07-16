/*
** EPITECH PROJECT, 2026
** SERVER
** File description:
** blacklist
*/

#ifndef BLACKLIST_H_
    #define BLACKLIST_H_

    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <stdbool.h>
    #include <stdlib.h>

typedef struct blacklist_s {
    struct in_addr ban_ip;
    struct blacklist_s *next;
} blacklist_t;

/**
 * @brief Adds a new banned IP address to the blacklist.
 *
 * Allocates a new blacklist_t node, stores the given IP address in it,
 * and inserts it at the head of the linked list. If the allocation
 * fails, the function does nothing and the list is left unchanged.
 *
 * @param head Pointer to the head of the blacklist linked list.
 * @param ip Pointer to the IP address to ban.
 *
 * @return None (void).
 */
void ban_ip(blacklist_t **head, const struct in_addr *ip);

/**
 * @brief Frees the entire blacklist linked list.
 *
 * Iterates over the linked list starting from head, freeing each
 * node one by one until the end of the list is reached.
 *
 * @param head Pointer to the head of the blacklist linked list to
 * destroy.
 *
 * @return None (void).
 */
void destroy_blacklist(blacklist_t *head);

/**
 * @brief Checks whether a given IP address is banned.
 *
 * Iterates over the blacklist linked list starting from head and
 * compares each stored IP address with the given one.
 *
 * @param head Pointer to the head of the blacklist linked list.
 * @param ip Pointer to the IP address to check.
 *
 * @return true if the IP address is found in the blacklist, false
 * otherwise.
 */
bool is_banned(blacklist_t *head, const struct in_addr *ip);

#endif
