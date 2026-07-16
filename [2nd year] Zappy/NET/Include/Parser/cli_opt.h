/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** cli_opt
*/

#ifndef CLI_OPT_H_
    #define CLI_OPT_H_

    #include <getopt.h>

static const struct option opt_options[] = {
    {"port", required_argument, 0, 'p'},
    {"width", required_argument, 0, 'x'},
    {"height", required_argument, 0, 'y'},
    {"clientsNb", required_argument, 0, 'c'},
    {"freq", required_argument, 0, 'f'},
    {"verbose", no_argument, 0, 'v'},
    {"team_names", required_argument, 0, 'n'},
    {"firewall", no_argument, 0, 'w'},
    {0, 0, 0, 0}
};

typedef struct attributs_s {
    char opt;
    char *arg_name;
    size_t min_value;
    size_t max_value;
} attributs_t;

static const attributs_t opts[] = {
    {'p', "port", 1024, 65535},
    {'x', "width", 10, 42},
    {'y', "height", 10, 42},
    {'f', "freq", 1, 150},
    {'c', "clientsNb", 1, 200}
};

#endif
