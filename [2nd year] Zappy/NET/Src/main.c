/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** main
*/

#include "server.h"

int main(int argc, char *argv[])
{
    server_t *server = NULL;

    if (display_help(argv))
        return EXIT_SUCCESS;
    logger_server("Initializing Zappy server...", OK);
    server = init_server(argc, argv);
    if (!server)
        return EXIT_FAILURE;
    display_config(server);
    run_server(server);
    destroy_server(server);
    return EXIT_SUCCESS;
}
