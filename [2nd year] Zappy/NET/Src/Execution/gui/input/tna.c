/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** tna
*/

#include "command.h"

void tna(__attribute__((unused)) world_t *world,
    __attribute__((unused)) gui_t *gui,
    __attribute__((unused)) char **args, int fd)
{
    for (unsigned int i = 0; world->args->team_names[i] != NULL; i++)
        logger_client(fd, "tna %s", world->args->team_names[i]);
}
