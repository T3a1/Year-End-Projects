/*
** EPITECH PROJECT, 2026
** SERVER
** File description:
** network
*/

#ifndef NETWORK_H_
    #define NETWORK_H_

    #include <poll.h>
    #include <stdint.h>
    #include <netinet/in.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>

    #include "logger.h"
    #include "connection.h"
    #include "blacklist.h"
    #include "server.h"

    #define MAX_CONNECTIONS 1023

    #define BUFFER_SIZE 1024

typedef struct connection_s connection_t;

typedef struct network_s {
    server_t *server;
    struct pollfd *poll_fds;
    struct sockaddr_in server_addr;
    connection_t *connections;
    blacklist_t *blacklist;
    size_t nb_connections;
    unsigned long max_connections;
} network_t;

/**
 * @brief Initializes the network structure and the server socket.
 *
 * Allocates the network_t structure, computes the maximum number of
 * connections, allocates and initializes the poll_fds array, creates
 * the listening socket, binds it to the given port and starts
 * listening for incoming connections.
 *
 * @param port Port number the server socket should listen on.
 *
 * @return A pointer to the newly allocated and initialized
 * network_t structure on success, or NULL if any allocation or
 * socket setup step failed.
 */
network_t *init_network(uint16_t port);

/**
 * @brief Frees the network structure and all its resources.
 *
 * Closes every open file descriptor stored in the poll_fds array,
 * destroys the connection linked list, destroys the blacklist linked
 * list, then frees the poll_fds array and the network_t structure
 * itself. Does nothing if the given pointer is NULL.
 *
 * @param network Pointer to the network_t structure to destroy.
 *
 * @return None (void).
 */
void destroy_network(network_t *network);

/**
 * @brief Accepts and handles a new incoming client connection.
 *
 * Accepts the pending connection on the listening socket. If the
 * server has reached its maximum number of connections, the client
 * is rejected via reject_full. If the firewall is enabled and the
 * client's IP address is banned, the client is rejected via
 * reject_banned. Otherwise, the connection is registered and set up
 * via setup_new_connection.
 *
 * @param network Pointer to the network_t structure holding the
 * listening socket, the connection list and the blacklist.
 * @param firewall Whether the firewall (IP banning) is enabled.
 *
 * @return None (void).
 */
void new_connection(network_t *network, bool firewall);

// Maybe change the attribut of the function
/**
 * @brief Reads a request from a client connection.
 *
 * Receives data from the socket associated with the connection at
 * the given index in the poll_fds array. If no data is received (the
 * client closed the connection or an error occurred), the connection
 * is disconnected via disconnect_connection.
 *
 * @param network Pointer to the network_t structure holding the
 * poll_fds array.
 * @param index Index of the connection in the poll_fds array.
 *
 * @return A newly allocated string containing the received request
 * on success, or NULL if no data was received and the connection
 * was disconnected.
 */
char *get_request(network_t *network, int index);

/**
 * @brief Disconnects a client connection from the network.
 *
 * Looks for the connection matching the file descriptor stored at
 * the given index in the poll_fds array. If found, removes it from
 * the connection list via remove_conn, closes its socket, frees its
 * resources, marks its poll_fds slot as unused and decrements the
 * number of active connections.
 *
 * @param network Pointer to the network_t structure holding the
 * connection list and the poll_fds array.
 * @param index Index of the connection to disconnect in the
 * poll_fds array.
 *
 * @return true if the connection was found and disconnected, false
 * otherwise.
 */
bool disconnect_connection(network_t *network, int index);

/**
 * @brief Disconnects a client connection from the network.
 *
 * Looks for the connection matching the file descriptor stored at
 * the given index in the poll_fds array. If found, removes it from
 * the connection list via remove_conn, closes its socket, frees its
 * resources, marks its poll_fds slot as unused and decrements the
 * number of active connections.
 *
 * @param network Pointer to the network_t structure holding the
 * connection list and the poll_fds array.
 * @param index Index of the connection to disconnect in the
 * poll_fds array.
 *
 * @return true if the connection was found and disconnected, false
 * otherwise.
 */
unsigned long get_max_connections(void);

#endif
