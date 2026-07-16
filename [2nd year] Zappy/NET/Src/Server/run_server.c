/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** run
*/

#include "server.h"
#include "network.h"
#include "signal.h"
#include "command.h"
#include "utils.h"
#include "parser.h"
#include <sys/resource.h>

volatile bool running = true;

static void handle_signal(int sig)
{
    (void)sig;
    logger_server("Shutting down Zappy server...", OK);
    running = false;
}

static void handle_connection(server_t *server)
{
    char **cmd = NULL;

    for (unsigned int i = 1; i < server->network->max_connections + 1; i++) {
        if (server->network->poll_fds[i].fd == -1)
            continue;
        if (server->network->poll_fds[i].revents &
            (POLLHUP | POLLERR | POLLNVAL)) {
            disconnect_connection(server->network, i);
            continue;
        }
        if (server->network->poll_fds[i].revents & POLLIN) {
            cmd = get_split_commands(server, i);
            execute_commands(server, cmd, i);
            free_array(cmd);
        }
    }
}

static bool handle_tick(server_t *server)
{
    refresh_ais(server->world, server->network->connections);
    refresh_commands(server);
    if (!win(server->world))
        return true;
    server->exit_status = 0;
    return false;
}

static void handle_poll_events(server_t *server)
{
    if (server->network->poll_fds[0].revents & POLLIN)
        new_connection(server->network, server->args->firewall);
    handle_connection(server);
}

static bool run_once(server_t *server)
{
    int timeout = server->world->next_command_time;
    nfds_t nfds = server->network->max_connections + 1;
    int ret = poll(server->network->poll_fds, nfds, timeout);

    if (ret < 0)
        return true;
    if (!handle_tick(server))
        return false;
    if (ret > 0)
        handle_poll_events(server);
    return true;
}

void run_server(server_t *server)
{
    signal(SIGINT, handle_signal);
    signal(SIGPIPE, SIG_IGN);
    while (running) {
        if (!run_once(server))
            break;
    }
}
