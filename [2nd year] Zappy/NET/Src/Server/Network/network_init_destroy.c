/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** init
*/

#include "network.h"
#include <sys/resource.h>

unsigned long get_max_connections(void)
{
    struct rlimit max_conn;

    getrlimit(RLIMIT_NOFILE, &max_conn);
    return (unsigned long) max_conn.rlim_cur - 1;
}

static bool bind_socket(struct sockaddr_in *addr, int server_fd, uint16_t port)
{
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = htonl(INADDR_ANY);
    addr->sin_port = htons(port);
    if (bind(server_fd, (struct sockaddr *)addr, sizeof(*addr)) == -1)
        return false;
    return true;
}

static bool init_pollfds(network_t *network)
{
    struct pollfd *array =
        calloc(network->max_connections + 1, sizeof(struct pollfd));

    if (!array)
        return false;
    network->poll_fds = array;
    return true;
}

static bool setup_server_socket
(int server_fd, network_t *network, uint16_t port)
{
    struct sockaddr_in addr = {0};
    int opt = 1;

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
            &opt, sizeof(opt)) == -1) {
        close(server_fd);
        return false;
    }
    if (!bind_socket(&addr, server_fd, port) || listen(server_fd, 10) == -1) {
        close(server_fd);
        return false;
    }
    for (unsigned long i = 0; i < network->max_connections + 1; i++) {
        network->poll_fds[i].fd = -1;
        network->poll_fds[i].events = POLLIN;
        network->poll_fds[i].revents = 0;
    }
    network->server_addr = addr;
    network->poll_fds[0].fd = server_fd;
    return true;
}

static bool fill_and_init_network(network_t *network, int *server_fd)
{
    network->max_connections = get_max_connections();
    if (!init_pollfds(network)) {
        free(network);
        return false;
    }
    *server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (*server_fd == -1) {
        free(network);
        return false;
    }
    network->connections = NULL;
    return true;
}

network_t *init_network(uint16_t port)
{
    network_t *network = calloc(1, sizeof(network_t));
    int server_fd = -1;

    if (!network)
        return NULL;
    if (!fill_and_init_network(network, &server_fd))
        return NULL;
    if (!setup_server_socket(server_fd, network, port)) {
        free(network);
        return NULL;
    }
    network->nb_connections = 0;
    network->blacklist = NULL;
    return network;
}

void destroy_network(network_t *network)
{
    if (!network)
        return;
    for (size_t i = 0; i < network->max_connections + 1; i++) {
        if (network->poll_fds[i].fd != -1)
            close(network->poll_fds[i].fd);
    }
    destroy_connections(network->connections);
    destroy_blacklist(network->blacklist);
    free(network->poll_fds);
    free(network);
}
