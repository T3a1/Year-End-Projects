/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** parse_integer
*/

#include "parser.h"
#include "cli_opt.h"

static size_t check_range(args_t *args, unsigned long n,
    const attributs_t *attribut)
{
    if (n < attribut->min_value || n > attribut->max_value) {
        logger_server("Error: %s's value must be between %u and %u", CRITICAL,
            attribut->arg_name, attribut->min_value, attribut->max_value);
        args->parsing_status = false;
        return 0;
    }
    return (size_t)n;
}

size_t parse_integer_arg(args_t *args, char *arg, const attributs_t *attribut)
{
    char *endptr = NULL;
    unsigned long n = 0;

    if (!arg || !arg[0]) {
        logger_server("Error: \"%s\" must be an integer", CRITICAL,
            attribut->arg_name);
        args->parsing_status = false;
        return 0;
    }
    n = strtoul(arg, &endptr, 10);
    if (*endptr != '\0') {
        logger_server("Error: \"%s\" must be an integer",
            CRITICAL, attribut->arg_name);
        args->parsing_status = false;
        return 0;
    }
    return check_range(args, n, attribut);
}
