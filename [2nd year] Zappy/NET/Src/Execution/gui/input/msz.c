/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** msz
*/

#include "command.h"

void msz(__attribute__((unused)) world_t *world,
    __attribute__((unused)) gui_t *gui,
    __attribute__((unused)) char **args, int fd)
{
    args_t *arg = world->args;

    logger_client(fd, "msz %d %d", arg->width, arg->height);
}
