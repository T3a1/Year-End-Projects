/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Unit tests for connection and bufferization functions
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>

#include "connection.h"
#include "parser.h"

#include "connection.h"

/*
** ─── HELPERS ────────────────────────────────────────────────────────────────
*/

static connection_t *make_conn(int fd)
{
    connection_t *c = calloc(1, sizeof(connection_t));

    c->fd = fd;
    c->partial_command = NULL;
    c->partial_len = 0;
    return c;
}

static network_t *make_network_with_pair(int fds[2])
{
    network_t *net = calloc(1, sizeof(network_t));

    net->poll_fds = calloc(MAX_CONNECTIONS + 1, sizeof(struct pollfd));
    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    net->poll_fds[1].fd = fds[0];
    net->poll_fds[1].events = POLLIN;
    net->nb_connections = 1;
    net->connections = make_conn(fds[0]);
    return net;
}

static void free_str_array(char **arr)
{
    if (!arr)
        return;
    for (int i = 0; arr[i]; i++)
        free(arr[i]);
    free(arr);
}

/*
** ─── get_connection ─────────────────────────────────────────────────────────
*/

Test(get_connection, returns_null_on_empty_list)
{
    cr_assert_null(get_connection(NULL, 42));
}

Test(get_connection, finds_only_element)
{
    connection_t *c = make_conn(5);
    connection_t *result = get_connection(c, 5);

    cr_assert_not_null(result);
    cr_assert_eq(result->fd, 5);
    free(c);
}

Test(get_connection, returns_null_when_fd_not_found)
{
    connection_t *c = make_conn(5);

    cr_assert_null(get_connection(c, 99));
    free(c);
}

Test(get_connection, finds_middle_element)
{
    connection_t *a = make_conn(1);
    connection_t *b = make_conn(2);
    connection_t *c = make_conn(3);

    a->next = b;
    b->next = c;
    cr_assert_eq(get_connection(a, 2)->fd, 2);
    free(a);
    free(b);
    free(c);
}

Test(get_connection, finds_last_element)
{
    connection_t *a = make_conn(1);
    connection_t *b = make_conn(2);
    connection_t *c = make_conn(3);

    a->next = b;
    b->next = c;
    cr_assert_eq(get_connection(a, 3)->fd, 3);
    free(a);
    free(b);
    free(c);
}

Test(get_connection, finds_first_element_in_chain)
{
    connection_t *a = make_conn(10);
    connection_t *b = make_conn(20);

    a->next = b;
    cr_assert_eq(get_connection(a, 10)->fd, 10);
    free(a);
    free(b);
}

/*
** ─── destroy_connections ────────────────────────────────────────────────────
*/

Test(destroy_connections, handles_null)
{
    /* Must not crash */
    destroy_connections(NULL);
}

Test(destroy_connections, frees_single_node)
{
    connection_t *c = make_conn(5);

    c->partial_command = strdup("hello");
    c->partial_len = 5;
    destroy_connections(c);
    /* valgrind will catch any leak */
}

Test(destroy_connections, frees_chain_of_three)
{
    connection_t *a = make_conn(1);
    connection_t *b = make_conn(2);
    connection_t *c = make_conn(3);

    a->next = b;
    b->next = c;
    b->partial_command = strdup("partial");
    b->partial_len = 7;
    destroy_connections(a);
}

/*
** ─── get_split_commands — socketpair-based tests ──────────────────────────────
*/

Test(get_split_commands, returns_null_on_closed_peer)
{
    int fds[2];
    network_t *net;
    server_t server;
    char **result;

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    net = make_network_with_pair(fds);
    server.network = net;
    close(fds[1]);         /* close writer side → recv returns 0 */
    result = get_split_commands(&server, 1);
    cr_assert_null(result);
    free(net->poll_fds);
    free(net);
}

Test(get_split_commands, single_complete_command)
{
    int fds[2];
    network_t *net = make_network_with_pair(fds);
    server_t server = { .network = net };
    char **result;

    write(fds[1], "hello\n", 6);
    close(fds[1]);
    result = get_split_commands(&server, 1);
    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "hello");
    cr_assert_null(result[1]);
    free_str_array(result);
    destroy_connections(net->connections);
    free(net->poll_fds);
    free(net);
}

Test(get_split_commands, multiple_complete_commands)
{
    int fds[2];
    network_t *net = make_network_with_pair(fds);
    server_t server = { .network = net };
    char **result;

    write(fds[1], "cmd1\ncmd2\ncmd3\n", 15);
    close(fds[1]);
    result = get_split_commands(&server, 1);
    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "cmd1");
    cr_assert_str_eq(result[1], "cmd2");
    cr_assert_str_eq(result[2], "cmd3");
    cr_assert_null(result[3]);
    free_str_array(result);
    destroy_connections(net->connections);
    free(net->poll_fds);
    free(net);
}

Test(get_split_commands, incomplete_command_returns_empty_array)
{
    int fds[2];
    network_t *net = make_network_with_pair(fds);
    server_t server = { .network = net };
    char **result;

    write(fds[1], "incom", 5);
    /* do NOT close fds[1] so recv does not get EOF */
    result = get_split_commands(&server, 1);
    cr_assert_not_null(result);
    cr_assert_null(result[0]);    /* NULL-terminated, zero commands */
    free_str_array(result);
    destroy_connections(net->connections);
    free(net->poll_fds);
    free(net);
    close(fds[1]);
}

Test(get_split_commands, partial_then_complete)
{
    int fds[2];
    network_t *net = make_network_with_pair(fds);
    server_t server = { .network = net };
    char **r1;
    char **r2;

    write(fds[1], "comm", 4);
    r1 = get_split_commands(&server, 1);
    cr_assert_not_null(r1);
    cr_assert_null(r1[0]);        /* nothing complete yet */
    free_str_array(r1);

    write(fds[1], "and\n", 4);
    r2 = get_split_commands(&server, 1);
    cr_assert_not_null(r2);
    cr_assert_str_eq(r2[0], "command");
    cr_assert_null(r2[1]);
    free_str_array(r2);

    destroy_connections(net->connections);
    free(net->poll_fds);
    free(net);
    close(fds[1]);
}

Test(get_split_commands, complex_split_scenario)
{
    int fds[2];
    network_t *net = make_network_with_pair(fds);
    server_t server = { .network = net };
    char **r1;
    char **r2;
    char **r3;

    /* Prompt 1: "command1\ncom" */
    write(fds[1], "command1\ncom", 12);
    r1 = get_split_commands(&server, 1);
    cr_assert_str_eq(r1[0], "command1");
    cr_assert_null(r1[1]);
    free_str_array(r1);

    /* Prompt 2: "mand2\ncommand3\ncommand4\nco" */
    write(fds[1], "mand2\ncommand3\ncommand4\nco", 26);
    r2 = get_split_commands(&server, 1);
    cr_assert_str_eq(r2[0], "command2");
    cr_assert_str_eq(r2[1], "command3");
    cr_assert_str_eq(r2[2], "command4");
    cr_assert_null(r2[3]);
    free_str_array(r2);

    /* Prompt 3: "mmand5\ncommand6" — command6 has no \n */
    write(fds[1], "mmand5\ncommand6", 15);
    r3 = get_split_commands(&server, 1);
    cr_assert_str_eq(r3[0], "command5");
    cr_assert_null(r3[1]);        /* command6 stays in partial */
    free_str_array(r3);

    destroy_connections(net->connections);
    free(net->poll_fds);
    free(net);
    close(fds[1]);
}

Test(get_split_commands, partial_persists_across_calls)
{
    int fds[2];
    network_t *net = make_network_with_pair(fds);
    server_t server = { .network = net };
    char **r;
    connection_t *conn = net->connections;

    write(fds[1], "abc", 3);
    r = get_split_commands(&server, 1);
    free_str_array(r);
    cr_assert_not_null(conn->partial_command);
    cr_assert_str_eq(conn->partial_command, "abc");

    destroy_connections(net->connections);
    free(net->poll_fds);
    free(net);
    close(fds[1]);
}

Test(get_split_commands, empty_command_between_newlines)
{
    int fds[2];
    network_t *net = make_network_with_pair(fds);
    server_t server = { .network = net };
    char **result;

    write(fds[1], "cmd1\n\ncmd2\n", 11);
    close(fds[1]);
    result = get_split_commands(&server, 1);
    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "cmd1");
    cr_assert_str_eq(result[1], "");      /* empty command between \n\n */
    cr_assert_str_eq(result[2], "cmd2");
    cr_assert_null(result[3]);
    free_str_array(result);
    destroy_connections(net->connections);
    free(net->poll_fds);
    free(net);
}

Test(get_split_commands, single_newline_only)
{
    int fds[2];
    network_t *net = make_network_with_pair(fds);
    server_t server = { .network = net };
    char **result;

    write(fds[1], "\n", 1);
    close(fds[1]);
    result = get_split_commands(&server, 1);
    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "");
    cr_assert_null(result[1]);
    free_str_array(result);
    destroy_connections(net->connections);
    free(net->poll_fds);
    free(net);
}

Test(get_split_commands, null_terminated_always)
{
    int fds[2];
    network_t *net = make_network_with_pair(fds);
    server_t server = { .network = net };
    char **result;
    int i = 0;

    write(fds[1], "a\nb\nc\n", 6);
    close(fds[1]);
    result = get_split_commands(&server, 1);
    cr_assert_not_null(result);
    while (result[i])
        i++;
    cr_assert_eq(i, 3);
    cr_assert_null(result[3]);
    free_str_array(result);
    destroy_connections(net->connections);
    free(net->poll_fds);
    free(net);
}

/*
** ─── add_connection ──────────────────────────────────────────────────────────
*/

static network_t *make_empty_network(void)
{
    network_t *net = calloc(1, sizeof(network_t));

    net->poll_fds = calloc(MAX_CONNECTIONS + 1, sizeof(struct pollfd));
    net->max_connections = MAX_CONNECTIONS;
    for (int i = 0; i < MAX_CONNECTIONS + 1; i++)
        net->poll_fds[i].fd = -1;
    net->connections = NULL;
    net->nb_connections = 0;
    return net;
}

static struct sockaddr_in make_addr(void)
{
    struct sockaddr_in addr = {0};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(4242);
    return addr;
}

Test(add_connection, returns_true_on_first_connection)
{
    network_t *net = make_empty_network();
    struct sockaddr_in addr = make_addr();

    cr_assert_eq(add_connection(net, 5, &addr), true);
    destroy_connections(net->connections);
    free(net->poll_fds);
    free(net);
}

Test(add_connection, connection_is_appended_to_list)
{
    network_t *net = make_empty_network();
    struct sockaddr_in addr = make_addr();

    add_connection(net, 5, &addr);
    cr_assert_not_null(net->connections);
    cr_assert_eq(net->connections->fd, 5);
    destroy_connections(net->connections);
    free(net->poll_fds);
    free(net);
}

Test(add_connection, second_connection_appended_at_tail)
{
    network_t *net = make_empty_network();
    struct sockaddr_in addr = make_addr();

    add_connection(net, 5, &addr);
    add_connection(net, 6, &addr);
    cr_assert_eq(net->connections->fd, 5);
    cr_assert_eq(net->connections->next->fd, 6);
    cr_assert_null(net->connections->next->next);
    destroy_connections(net->connections);
    free(net->poll_fds);
    free(net);
}

Test(add_connection, three_connections_order_preserved)
{
    network_t *net = make_empty_network();
    struct sockaddr_in addr = make_addr();

    add_connection(net, 10, &addr);
    add_connection(net, 20, &addr);
    add_connection(net, 30, &addr);
    cr_assert_eq(net->connections->fd, 10);
    cr_assert_eq(net->connections->next->fd, 20);
    cr_assert_eq(net->connections->next->next->fd, 30);
    cr_assert_null(net->connections->next->next->next);
    destroy_connections(net->connections);
    free(net->poll_fds);
    free(net);
}

Test(add_connection, poll_fd_is_registered)
{
    network_t *net = make_empty_network();
    struct sockaddr_in addr = make_addr();
    bool found = false;

    add_connection(net, 42, &addr);
    for (int i = 0; i < MAX_CONNECTIONS + 1; i++)
        if (net->poll_fds[i].fd == 42)
            found = true;
    cr_assert_eq(found, true);
    destroy_connections(net->connections);
    free(net->poll_fds);
    free(net);
}

Test(add_connection, poll_fd_uses_first_available_slot)
{
    network_t *net = make_empty_network();
    struct sockaddr_in addr = make_addr();

    net->poll_fds[0].fd = 99;  /* slot 0 taken */
    add_connection(net, 42, &addr);
    cr_assert_eq(net->poll_fds[1].fd, 42);
    destroy_connections(net->connections);
    free(net->poll_fds);
    free(net);
}

Test(add_connection, addr_is_copied_correctly)
{
    network_t *net = make_empty_network();
    struct sockaddr_in addr = make_addr();

    addr.sin_port = htons(1234);
    add_connection(net, 5, &addr);
    cr_assert_eq(net->connections->addr.sin_port, htons(1234));
    destroy_connections(net->connections);
    free(net->poll_fds);
    free(net);
}

Test(add_connection, type_is_unknown_after_init)
{
    network_t *net = make_empty_network();
    struct sockaddr_in addr = make_addr();

    add_connection(net, 5, &addr);
    cr_assert_eq(net->connections->type, UNKNOWN);
    destroy_connections(net->connections);
    free(net->poll_fds);
    free(net);
}

Test(add_connection, partial_command_is_null_after_init)
{
    network_t *net = make_empty_network();
    struct sockaddr_in addr = make_addr();

    add_connection(net, 5, &addr);
    cr_assert_null(net->connections->partial_command);
    cr_assert_eq(net->connections->partial_len, 0);
    destroy_connections(net->connections);
    free(net->poll_fds);
    free(net);
}

Test(add_connection, cli_fields_are_null_after_init)
{
    network_t *net = make_empty_network();
    struct sockaddr_in addr = make_addr();

    add_connection(net, 5, &addr);
    cr_assert_null(net->connections->cli.ai);
    destroy_connections(net->connections);
    free(net->poll_fds);
    free(net);
}

Test(add_connection, next_is_null_on_single_connection)
{
    network_t *net = make_empty_network();
    struct sockaddr_in addr = make_addr();

    add_connection(net, 5, &addr);
    cr_assert_null(net->connections->next);
    destroy_connections(net->connections);
    free(net->poll_fds);
    free(net);
}

/*
** ─── free_array ───────────────────────────────────────────────────────
*/

Test(free_array, handles_null)
{
    /* must not crash */
    free_array(NULL);
}

Test(free_array, handles_empty_null_terminated_array)
{
    char **arr = malloc(sizeof(char *));

    arr[0] = NULL;
    free_array(arr);
    /* valgrind catches any leak */
}

Test(free_array, frees_single_element)
{
    char **arr = malloc(sizeof(char *) * 2);

    arr[0] = strdup("hello");
    arr[1] = NULL;
    free_array(arr);
}

Test(free_array, frees_multiple_elements)
{
    char **arr = malloc(sizeof(char *) * 4);

    arr[0] = strdup("cmd1");
    arr[1] = strdup("cmd2");
    arr[2] = strdup("cmd3");
    arr[3] = NULL;
    free_array(arr);
}

Test(free_array, frees_empty_string_element)
{
    char **arr = malloc(sizeof(char *) * 2);

    arr[0] = strdup("");
    arr[1] = NULL;
    free_array(arr);
}

/*
** ─── disconnect_connection ───────────────────────────────────────────────────
*/

static server_t *make_bare_server(void)
{
    server_t *srv = calloc(1, sizeof(server_t));
    world_t *world = calloc(1, sizeof(world_t));
    args_t *args = calloc(1, sizeof(args_t));

    args->width = 5; args->height = 5; args->freq = 1;
    world->args = args;
    world->gui = NULL;
    world->ais = NULL;
    world->teams = NULL;
    world->command_queue = NULL;
    world->connections = NULL;
    srv->world = world;
    return srv;
}

static network_t *make_network_for_disconnect(server_t *srv, int fd)
{
    network_t *net = calloc(1, sizeof(network_t));
    connection_t *conn = calloc(1, sizeof(connection_t));

    net->poll_fds = calloc(MAX_CONNECTIONS + 1, sizeof(struct pollfd));
    net->max_connections = MAX_CONNECTIONS;
    net->poll_fds[1].fd = fd;
    conn->fd = fd;
    conn->type = UNKNOWN;
    conn->cli.ai = NULL;
    conn->partial_command = NULL;
    conn->next = NULL;
    net->connections = conn;
    net->nb_connections = 1;
    net->server = srv;
    srv->network = net;
    return net;
}

Test(disconnect_connection, unknown_type_disconnects)
{
    int fds[2];
    server_t *srv = make_bare_server();

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    network_t *net = make_network_for_disconnect(srv, fds[0]);

    bool result = disconnect_connection(net, 1);
    cr_assert_eq(result, true);
    cr_assert_eq(net->nb_connections, 0);
    cr_assert_eq(net->poll_fds[1].fd, -1);
    /* fds[0] was closed by disconnect_connection */
    close(fds[1]);
    free(net->poll_fds);
    free(net);
    free(srv->world->args);
    free(srv->world);
    free(srv);
}

Test(disconnect_connection, returns_false_when_fd_not_found)
{
    int fds[2];
    server_t *srv = make_bare_server();

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    network_t *net = make_network_for_disconnect(srv, fds[0]);
    net->poll_fds[1].fd = 999;  /* mismatch → won't find connection */

    bool result = disconnect_connection(net, 1);
    cr_assert_eq(result, false);
    close(fds[0]); close(fds[1]);
    destroy_connections(net->connections);
    free(net->poll_fds);
    free(net);
    free(srv->world->args);
    free(srv->world);
    free(srv);
}

/*
** ─── disconnect_connection: GUI and second-in-list ───────────────────────────
*/

Test(disconnect_connection, second_connection_in_list)
{
    int fds[2];
    server_t *srv = make_bare_server();

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);

    network_t *net = calloc(1, sizeof(network_t));
    net->poll_fds = calloc(MAX_CONNECTIONS + 1, sizeof(struct pollfd));
    net->max_connections = MAX_CONNECTIONS;
    net->server = srv;
    srv->network = net;

    /* two connections: fds[0] first, then a dummy */
    connection_t *c1 = calloc(1, sizeof(connection_t));
    connection_t *c2 = calloc(1, sizeof(connection_t));
    c1->fd = 99; c1->type = UNKNOWN; c1->next = c2;
    c2->fd = fds[0]; c2->type = UNKNOWN; c2->next = NULL;
    net->connections = c1;
    net->nb_connections = 2;
    net->poll_fds[1].fd = fds[0];

    bool result = disconnect_connection(net, 1);
    cr_assert_eq(result, true);
    cr_assert_eq(c1->next, NULL);   /* c2 was removed */
    cr_assert_eq(net->nb_connections, 1);

    close(fds[1]);
    free(c1);
    free(net->poll_fds);
    free(net);
    free(srv->world->args);
    free(srv->world);
    free(srv);
}
