/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Unit tests for parser functions
*/

#include <criterion/criterion.h>
#include <string.h>
#include <stdlib.h>

#include "parser.h"
#include "args.h"
#include "cli_opt.h"

/*
** ─── HELPERS ─────────────────────────────────────────────────────────────────
*/

static args_t *make_valid_args(void)
{
    args_t *args = init_args();

    args->parsing_status = true;
    return args;
}

/*
** ─── init_args / destroy_args ────────────────────────────────────────────────
*/

Test(init_args, returns_non_null)
{
    args_t *a = init_args();

    cr_assert_not_null(a);
    destroy_args(a);
}

Test(init_args, parsing_status_is_true)
{
    args_t *a = init_args();

    cr_assert_eq(a->parsing_status, true);
    destroy_args(a);
}

Test(init_args, default_port_is_1234)
{
    args_t *a = init_args();

    cr_assert_eq(a->port, 1234);
    destroy_args(a);
}

Test(init_args, default_width_is_26)
{
    args_t *a = init_args();

    cr_assert_eq(a->width, 26u);
    destroy_args(a);
}

Test(init_args, default_height_is_26)
{
    args_t *a = init_args();

    cr_assert_eq(a->height, 26u);
    destroy_args(a);
}

Test(init_args, team_names_not_null)
{
    args_t *a = init_args();

    cr_assert_not_null(a->team_names);
    cr_assert_not_null(a->team_names[0]);
    cr_assert_null(a->team_names[1]);
    destroy_args(a);
}

Test(init_args, firewall_is_false)
{
    args_t *a = init_args();

    cr_assert_eq(a->firewall, false);
    destroy_args(a);
}

Test(destroy_args, handles_null)
{
    /* must not crash */
    destroy_args(NULL);
}

Test(destroy_args, frees_valid_args)
{
    args_t *a = init_args();

    destroy_args(a);
    /* valgrind catches any leak */
}

/*
** ─── parse_integer_arg ───────────────────────────────────────────────────────
*/

Test(parse_integer_arg, valid_port_in_range)
{
    args_t *args = make_valid_args();
    size_t result = parse_integer_arg(args, "4242", &opts[0]);

    cr_assert_eq(result, 4242u);
    cr_assert_eq(args->parsing_status, true);
    destroy_args(args);
}

Test(parse_integer_arg, below_min_sets_status_false)
{
    args_t *args = make_valid_args();

    parse_integer_arg(args, "1", &opts[0]);
    cr_assert_eq(args->parsing_status, false);
    destroy_args(args);
}

Test(parse_integer_arg, above_max_sets_status_false)
{
    args_t *args = make_valid_args();

    parse_integer_arg(args, "99999", &opts[0]);
    cr_assert_eq(args->parsing_status, false);
    destroy_args(args);
}

Test(parse_integer_arg, non_numeric_sets_status_false)
{
    args_t *args = make_valid_args();

    parse_integer_arg(args, "abc", &opts[0]);
    cr_assert_eq(args->parsing_status, false);
    destroy_args(args);
}

Test(parse_integer_arg, empty_string_sets_status_false)
{
    args_t *args = make_valid_args();

    parse_integer_arg(args, "", &opts[0]);
    cr_assert_eq(args->parsing_status, false);
    destroy_args(args);
}

Test(parse_integer_arg, returns_zero_on_non_numeric)
{
    args_t *args = make_valid_args();
    size_t result = parse_integer_arg(args, "xyz", &opts[0]);

    cr_assert_eq(result, 0u);
    destroy_args(args);
}

Test(parse_integer_arg, valid_width_10)
{
    args_t *args = make_valid_args();
    size_t result = parse_integer_arg(args, "10", &opts[1]);

    cr_assert_eq(result, 10u);
    cr_assert_eq(args->parsing_status, true);
    destroy_args(args);
}

Test(parse_integer_arg, valid_freq_boundary)
{
    args_t *args = make_valid_args();
    size_t result = parse_integer_arg(args, "10000", &opts[3]);

    cr_assert_eq(result, 10000u);
    cr_assert_eq(args->parsing_status, true);
    destroy_args(args);
}

/*
** ─── parse_player_number ─────────────────────────────────────────────────────
*/

Test(parse_player_number, valid_hash_zero)
{
    cr_assert_eq(parse_player_number("#0"), 0);
}

Test(parse_player_number, valid_hash_one)
{
    cr_assert_eq(parse_player_number("#1"), 1);
}

Test(parse_player_number, valid_large_id)
{
    cr_assert_eq(parse_player_number("#42"), 42);
}

Test(parse_player_number, no_hash_prefix_returns_minus_one)
{
    cr_assert_eq(parse_player_number("42"), -1);
}

Test(parse_player_number, null_returns_minus_one)
{
    cr_assert_eq(parse_player_number(NULL), -1);
}

Test(parse_player_number, alpha_after_hash_returns_minus_one)
{
    cr_assert_eq(parse_player_number("#abc"), -1);
}

Test(parse_player_number, empty_after_hash_returns_minus_one)
{
    cr_assert_eq(parse_player_number("#"), -1);
}

Test(parse_player_number, mixed_after_hash_returns_minus_one)
{
    cr_assert_eq(parse_player_number("#3x"), -1);
}
