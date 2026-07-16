/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Unit tests for utils functions
*/

#include <criterion/criterion.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "connection.h"
#include "command.h"

/*
** ─── get_integer ─────────────────────────────────────────────────────────────
*/

Test(get_integer, parses_positive_number)
{
    int val = 0;

    cr_assert_eq(get_integer("42", &val), true);
    cr_assert_eq(val, 42);
}

Test(get_integer, parses_zero)
{
    int val = 99;

    cr_assert_eq(get_integer("0", &val), true);
    cr_assert_eq(val, 0);
}

Test(get_integer, parses_negative_number)
{
    int val = 0;

    cr_assert_eq(get_integer("-5", &val), true);
    cr_assert_eq(val, -5);
}

Test(get_integer, rejects_alpha_string)
{
    int val = 0;

    cr_assert_eq(get_integer("abc", &val), false);
}

Test(get_integer, rejects_mixed_string)
{
    int val = 0;

    cr_assert_eq(get_integer("12abc", &val), false);
}

Test(get_integer, rejects_empty_string)
{
    int val = 0;

    cr_assert_eq(get_integer("", &val), false);
}

Test(get_integer, parses_int_max)
{
    int val = 0;

    cr_assert_eq(get_integer("2147483647", &val), true);
    cr_assert_eq(val, 2147483647);
}

/*
** ─── int_to_str ──────────────────────────────────────────────────────────────
*/

Test(int_to_str, converts_positive)
{
    char *s = int_to_str(42);

    cr_assert_not_null(s);
    cr_assert_str_eq(s, "42");
    free(s);
}

Test(int_to_str, converts_zero)
{
    char *s = int_to_str(0);

    cr_assert_not_null(s);
    cr_assert_str_eq(s, "0");
    free(s);
}

Test(int_to_str, converts_negative)
{
    char *s = int_to_str(-7);

    cr_assert_not_null(s);
    cr_assert_str_eq(s, "-7");
    free(s);
}

Test(int_to_str, returns_heap_allocated)
{
    char *s = int_to_str(1);

    cr_assert_not_null(s);
    free(s);  /* valgrind: must not crash */
}

/*
** ─── split_arguments ─────────────────────────────────────────────────────────
*/

Test(split_arguments, splits_two_tokens)
{
    char input[] = "Take food";
    char **result = split_arguments(input, " ");

    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "Take");
    cr_assert_str_eq(result[1], "food");
    cr_assert_null(result[2]);
    free_array(result);
}

Test(split_arguments, single_token)
{
    char input[] = "Forward";
    char **result = split_arguments(input, " ");

    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "Forward");
    cr_assert_null(result[1]);
    free_array(result);
}

Test(split_arguments, three_tokens)
{
    char input[] = "a b c";
    char **result = split_arguments(input, " ");

    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "a");
    cr_assert_str_eq(result[1], "b");
    cr_assert_str_eq(result[2], "c");
    cr_assert_null(result[3]);
    free_array(result);
}

Test(split_arguments, returns_null_on_empty_string)
{
    char input[] = "";
    char **result = split_arguments(input, " ");

    cr_assert_null(result);
}

Test(split_arguments, custom_delimiter)
{
    char input[] = "x,y,z";
    char **result = split_arguments(input, ",");

    cr_assert_not_null(result);
    cr_assert_str_eq(result[0], "x");
    cr_assert_str_eq(result[1], "y");
    cr_assert_str_eq(result[2], "z");
    cr_assert_null(result[3]);
    free_array(result);
}

/*
** ─── get_fd_from_connections ─────────────────────────────────────────────────
*/

static connection_t *make_conn_with_ai(int fd, ai_t *ai)
{
    connection_t *c = calloc(1, sizeof(connection_t));

    c->fd = fd;
    c->type = AI;
    c->cli.ai = ai;
    c->partial_command = NULL;
    c->partial_len = 0;
    return c;
}

Test(get_fd_from_connections, returns_minus_one_on_null_list)
{
    ai_t ai = {0};

    cr_assert_eq(get_fd_from_connections(NULL, &ai), -1);
}

Test(get_fd_from_connections, finds_fd_for_ai)
{
    ai_t ai = {0};
    connection_t *c = make_conn_with_ai(7, &ai);

    cr_assert_eq(get_fd_from_connections(c, &ai), 7);
    free(c);
}

Test(get_fd_from_connections, returns_minus_one_when_not_found)
{
    ai_t ai1 = {0};
    ai_t ai2 = {0};
    connection_t *c = make_conn_with_ai(7, &ai1);

    cr_assert_eq(get_fd_from_connections(c, &ai2), -1);
    free(c);
}

Test(get_fd_from_connections, finds_correct_fd_in_chain)
{
    ai_t ai1 = {0};
    ai_t ai2 = {0};
    connection_t *c1 = make_conn_with_ai(3, &ai1);
    connection_t *c2 = make_conn_with_ai(9, &ai2);

    c1->next = c2;
    cr_assert_eq(get_fd_from_connections(c1, &ai2), 9);
    free(c1);
    free(c2);
}

/*
** ─── count_args ──────────────────────────────────────────────────────────────
*/

Test(count_args, zero_args_after_command_name)
{
    char *args[] = {"Forward", NULL};

    cr_assert_eq(count_args(args), 0);
}

Test(count_args, one_arg)
{
    char *args[] = {"Take", "food", NULL};

    cr_assert_eq(count_args(args), 1);
}

Test(count_args, two_args)
{
    char *args[] = {"Broadcast", "Hello world", NULL};

    cr_assert_eq(count_args(args), 1);
}

Test(count_args, three_tokens_two_args)
{
    char *args[] = {"cmd", "a", "b", NULL};

    cr_assert_eq(count_args(args), 2);
}
