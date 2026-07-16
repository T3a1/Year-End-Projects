/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** display_config
*/

#include "server.h"
#include "network.h"

void display_config(server_t *server)
{
    args_t *args = server->args;

    printf(BOLD "===================Zappy Server===================\n");
    printf("port = %u\n", args->port);
    printf("max_connections = %zu\n", server->network->max_connections + 1);
    printf("width = %zu\n", args->width);
    printf("height = %zu\n", args->height);
    printf("eggs_per_team = %zu\n", args->eggs_per_team);
    printf("freq = %zu\n", args->freq);
    printf("nameX = ");
    for (unsigned int i = 0; args->team_names[i] != NULL; i++)
        printf("%s ", args->team_names[i]);
    printf("\n");
    printf("firewall = %d\n", args->firewall);
    printf("verbose = %d\n", args->verbose);
    printf("==================================================\n" RESET);
}
