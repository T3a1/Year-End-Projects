/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** server
*/

#ifndef SERVER_H_
    #define SERVER_H_

    #undef EXIT_FAILURE
    #define EXIT_FAILURE 84

    #include <stdlib.h>

    #include "args.h"
    #include "logger.h"
    #include "world.h"

typedef struct network_s network_t;

typedef struct server_s {
    args_t *args;
    network_t *network;
    world_t *world;
    int exit_status;
} server_t;

/**
 * @brief Initializes the server structure and all its components.
 *
 * Allocates the server_t structure, parses the command line
 * arguments, initializes the network (socket, poll_fds, etc.) and
 * the world, then links them together (the network keeps a back
 * reference to the server, and the world keeps a reference to the
 * connection list).
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line argument strings.
 *
 * @return A pointer to the newly allocated and initialized server_t
 * structure on success, or NULL if the server, the arguments or the
 * network failed to initialize.
 */
server_t *init_server(int argc, char *argv[]);

/**
 * @brief Frees the server structure and all its components.
 *
 * Destroys the network, the world and the parsed arguments, then
 * frees the server_t structure itself. Does nothing if the given
 * pointer is NULL.
 *
 * @param server Pointer to the server_t structure to destroy.
 *
 * @return None (void).
 */
void destroy_server(server_t *server);

/**
 * @brief Runs the main server loop.
 *
 * Registers a SIGINT handler so the server can shut down gracefully,
 * then repeatedly calls run_once (which polls for events, advances
 * the game tick and handles new connections/commands) until the
 * running flag is cleared or run_once signals that the loop should
 * stop.
 *
 * @param server Pointer to the server_t structure to run.
 *
 * @return None (void).
 */
void run_server(server_t *server);

#endif
