/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** display_help
*/

#include "logger.h"

bool display_help(char *argv[])
{
    for (size_t i = 0; argv[i] != NULL; i++)
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            display_usage();
            printf("    port            is the port number\n");
            printf("    width           is the width of the world\n");
            printf("    height          is the height of the world\n");
            printf("    nameX           is the name of the team X\n");
            printf("    clientsNb       is the number of authorized ");
            printf("clients per team\n");
            printf("    freq            is the reciprocal of time unit ");
            printf("for execution of actions\n");
            printf("    --firewall      is a flag to enable firewall\n");
            printf("    --verbose       is a flag to enable verbose mode\n");
            printf("    --help          is a flag to display ");
            printf("this help message\n");
            return true;
        }
    return false;
}
