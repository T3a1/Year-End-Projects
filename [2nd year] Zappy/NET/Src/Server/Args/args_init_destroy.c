/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** init_destroy
*/

#include "args.h"

args_t *init_args(void)
{
    args_t *args = calloc(1, sizeof(args_t));

    if (!args)
        return NULL;
    args->port = 1234;
    args->width = 26;
    args->height = 26;
    args->eggs_per_team = 100;
    args->freq = 100;
    args->team_names = malloc(sizeof(char *) * 2);
    if (!args->team_names) {
        free(args);
        return NULL;
    }
    args->team_names[0] = strdup("TGEgYnJhbmxlIGRlcyBzb3VyaXMK");
    args->team_names[1] = NULL;
    args->firewall = false;
    args->parsing_status = true;
    args->verbose = false;
    return args;
}

void destroy_args(args_t *args)
{
    if (!args)
        return;
    if (args->team_names)
        free_array(args->team_names);
    free(args);
}
