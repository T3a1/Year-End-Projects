/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** new_connection
*/

#include "network.h"

// Maybe adding ip and port in the log message
static void setup_new_connection
(network_t *network, struct sockaddr_in *client_addr, int client_fd)
{
    if (add_connection(network, client_fd, client_addr)) {
        logger_server("New client connected %s:%d", INFO,
            inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));
        logger_client(client_fd, "WELCOME");
        network->nb_connections++;
    } else {
        close(client_fd);
    }
}

// Maybe adding ip and port in the log message
// Change is_ip_banned to is_banned
static void reject_full(int fd, struct sockaddr_in *addr)
{
    logger_client(fd, "Server is full");
    logger_server("Client %s:%d tried to connect but server is full",
        MINOR, inet_ntoa(addr->sin_addr), ntohs(addr->sin_port));
    close(fd);
}

static void reject_banned(int fd, struct sockaddr_in *addr)
{
    logger_client(fd, "You're banned from this server");
    logger_server("Banned client %s:%d tried to connect back",
        WARNING, inet_ntoa(addr->sin_addr), ntohs(addr->sin_port));
    close(fd);
}

static bool is_connection_banned(network_t *network,
    struct sockaddr_in *addr, bool firewall)
{
    return firewall && is_banned(network->blacklist, &addr->sin_addr);
}

void new_connection(network_t *network, bool firewall)
{
    struct sockaddr_in addr;
    socklen_t len;
    int fd;

    len = sizeof(addr);
    fd = accept(network->poll_fds[0].fd, (struct sockaddr *)&addr, &len);
    if (fd < 0)
        return;
    if (network->nb_connections >= network->max_connections) {
        reject_full(fd, &addr);
        return;
    }
    if (is_connection_banned(network, &addr, firewall)) {
        reject_banned(fd, &addr);
        return;
    }
    setup_new_connection(network, &addr, fd);
}
