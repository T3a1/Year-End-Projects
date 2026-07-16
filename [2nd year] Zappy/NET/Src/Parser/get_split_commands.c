/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** split_commands
*/

#include "parser.h"
#include "network.h"
#include "server.h"

static char **append_command(char **cmds, int *count, char *cmd)
{
    char **new_cmds = realloc(cmds, sizeof(char *) * (*count + 2));

    if (!new_cmds)
        return cmds;
    new_cmds[*count] = strdup(cmd);
    new_cmds[*count + 1] = NULL;
    (*count)++;
    return new_cmds;
}

static char **extract_commands(char *buf, int *count)
{
    char **cmds = malloc(sizeof(char *));
    char *start = buf;
    char *end = NULL;

    if (!cmds)
        return NULL;
    cmds[0] = NULL;
    end = strchr(start, '\n');
    while (end) {
        *end = '\0';
        cmds = append_command(cmds, count, start);
        start = end + 1;
        end = strchr(start, '\n');
    }
    memmove(buf, start, strlen(start) + 1);
    return cmds;
}

static bool grow_partial(connection_t *conn, size_t needed)
{
    size_t new_size = conn->partial_len + needed + 1;
    char *new_buf = realloc(conn->partial_command, new_size);

    if (!new_buf)
        return false;
    conn->partial_command = new_buf;
    return true;
}

static char **flush_partial(connection_t *conn, char *buf, int *count)
{
    char **cmds = NULL;

    if (!grow_partial(conn, strlen(buf))) {
        free(conn->partial_command);
        conn->partial_command = NULL;
        conn->partial_len = 0;
        return NULL;
    }
    strcat(conn->partial_command, buf);
    conn->partial_len = strlen(conn->partial_command);
    cmds = extract_commands(conn->partial_command, count);
    conn->partial_len = strlen(conn->partial_command);
    return cmds;
}

char **get_split_commands(server_t *server, int index)
{
    network_t *net = server->network;
    char buffer[BUFFER_SIZE] = {0};
    ssize_t bytes_read = recv(net->poll_fds[index].fd,
        buffer, BUFFER_SIZE - 1, 0);
    connection_t *conn = get_connection(net->connections,
        net->poll_fds[index].fd);
    int count = 0;

    if (bytes_read <= 0) {
        disconnect_connection(server->network, index);
        return NULL;
    }
    buffer[bytes_read] = '\0';
    if (!conn->partial_command) {
        conn->partial_command = strdup("");
        conn->partial_len = 0;
    }
    return flush_partial(conn, buffer, &count);
}
