/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** init_destroy
*/

#include "server.h"
#include "network.h"
#include "parser.h"

static void init_world(server_t *server)
{
    server->world = create_world(server->args);
    if (!server->world) {
        destroy_args(server->args);
        destroy_network(server->network);
        free(server);
        server = NULL;
        return;
    }
}

server_t *init_server(int argc, char *argv[])
{
    server_t *server = calloc(1, sizeof(server_t));

    if (!server)
        return NULL;
    server->exit_status = EXIT_SUCCESS;
    server->args = parse_args(argc, argv);
    if (!server->args) {
        free(server);
        return NULL;
    }
    server->network = init_network(server->args->port);
    if (!server->network) {
        destroy_args(server->args);
        free(server);
        return NULL;
    }
    server->network->server = server;
    init_world(server);
    server->world->connections = &server->network->connections;
    return server;
}

void destroy_server(server_t *server)
{
    if (!server)
        return;
    destroy_network(server->network);
    destroy_world(server->world);
    destroy_args(server->args);
    free(server);
}
