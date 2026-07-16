/*
** EPITECH PROJECT, 2026
** SERVER
** File description:
** parse_args
*/

#include "parser.h"
#include "cli_opt.h"

static void second_opt_case(args_t *args, int opt, int argc, char *argv[])
{
    switch (opt) {
        case 'n':
            args->team_names = parse_team_names(args, argv, argc, &optind);
            break;
        case 'w':
            args->firewall = true;
            break;
        case 'v':
            args->verbose = true;
            break;
        case '?':
            args->parsing_status = false;
            break;
    }
}

static void first_opt_case(args_t *args, int opt, int argc, char *argv[])
{
    switch (opt) {
        case 'p':
            args->port = parse_integer_arg(args, optarg, &opts[0]);
            break;
        case 'x':
            args->width = parse_integer_arg(args, optarg, &opts[1]);
            break;
        case 'y':
            args->height = parse_integer_arg(args, optarg, &opts[2]);
            break;
        case 'f':
            args->freq = parse_integer_arg(args, optarg, &opts[3]);
            break;
        case 'c':
            args->eggs_per_team = parse_integer_arg(args, optarg, &opts[4]);
            break;
        default:
            second_opt_case(args, opt, argc, argv);
            break;
    }
}

static bool unknown_argument(args_t *args, char *argv[], int argc, int optind)
{
    if (optind < argc) {
        logger_server("Unexpected argument: %s", CRITICAL, argv[optind]);
        destroy_args(args);
        display_usage();
        return true;
    }
    return false;
}

args_t *parse_args(int argc, char *argv[])
{
    int opt = 0;
    args_t *args = init_args();

    if (!args)
        return NULL;
    opt = getopt_long(argc, argv, "p:x:y:n:c:f:vw", opt_options, NULL);
    while (opt != -1) {
        first_opt_case(args, opt, argc, argv);
        if (!args->parsing_status) {
            destroy_args(args);
            display_usage();
            return NULL;
        }
        opt = getopt_long(argc, argv, "p:x:y:n:c:f:vw", opt_options, NULL);
    }
    if (unknown_argument(args, argv, argc, optind))
        return NULL;
    return args;
}
