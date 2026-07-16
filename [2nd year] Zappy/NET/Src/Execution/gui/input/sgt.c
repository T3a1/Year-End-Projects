/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** sgt
*/

#include "command.h"

void sgt(world_t *world, __attribute__((unused)) gui_t *gui,
    __attribute__((unused)) char **args, int fd)
{
    logger_client(fd, "sgt %d", world->args->freq);
}
