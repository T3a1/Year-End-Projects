/*
** EPITECH PROJECT, 2026
** SERVER
** File description:
** connection
*/

#ifndef CONNECTION_H_
    #define CONNECTION_H_

    #include <netinet/in.h>
    #include <stdbool.h>

    #include "resources.h"

    #include "ai.h"
    #include "gui.h"
    #include "network.h"

typedef enum {
    GUI,
    AI,
    UNKNOWN
} client_type_t;

typedef union client_data_u {
    ai_t *ai;
    gui_t *gui;
} client_data_t;

typedef struct network_s network_t;

typedef struct connection_s {
    int fd;
    struct sockaddr_in addr;
    client_type_t type;
    client_data_t cli;
    char *partial_command;
    size_t partial_len;
    struct connection_s *next;
} connection_t;

/**
 * @brief Adds a new connection to the network's connection list.
 *
 * Creates a new connection_t node for the given file descriptor and
 * address, registers it in the first available slot of the
 * network's poll_fds array, then appends it to the end of the
 * network's connection linked list.
 *
 * @param network Pointer to the network_t structure holding the
 * connection list and the poll_fds array.
 * @param fd File descriptor of the new connection.
 * @param addr Pointer to the socket address of the new connection.
 *
 * @return true if the connection was successfully created and
 * added, false if the connection_t allocation failed.
 */
bool add_connection(network_t *network, int fd, const struct sockaddr_in *addr);

/**
 * @brief Frees the entire connection linked list.
 *
 * Iterates over the linked list starting from head, freeing the
 * partial_command buffer of each node when allocated, then freeing
 * the node itself, until the end of the list is reached.
 *
 * @param head Pointer to the head of the connection linked list to
 * destroy.
 *
 * @return None (void).
 */
void destroy_connections(connection_t *head);


/**
 * @brief Finds a connection by its file descriptor.
 *
 * Iterates over the connection linked list starting from head and
 * looks for the node whose fd field matches the given file
 * descriptor.
 *
 * @param head Pointer to the head of the connection linked list.
 * @param fd File descriptor to search for.
 *
 * @return A pointer to the matching connection_t structure if
 * found, or NULL otherwise.
 */
connection_t *get_connection(connection_t *head, int fd);

/**
 * @brief Retrieves the file descriptor associated with an AI client.
 *
 * Iterates over the connection linked list and looks for the node
 * whose cli.ai pointer matches the given ai pointer.
 *
 * @param connections Pointer to the head of the connection linked
 * list.
 * @param ai Pointer to the ai_t structure to search for.
 *
 * @return The file descriptor of the matching connection if found,
 * or -1 otherwise.
 */
int get_fd_from_connections(connection_t *connections, ai_t *ai);

#endif
