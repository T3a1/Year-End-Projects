/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** mct
*/

#include "command.h"

static void mct_row(world_t *world, gui_t *gui, int fd, char *x_str)
{
    char *y_str = NULL;

    for (size_t j = 0; j < world->args->height; j++) {
        y_str = int_to_str(j);
        if (!y_str)
            continue;
        bct(world, gui, (char *[]){"bct", x_str, y_str, NULL}, fd);
        free(y_str);
    }
}

void mct(__attribute__((unused)) world_t *world,
    __attribute__((unused)) gui_t *gui,
    __attribute__((unused)) char **args, int fd)
{
    char *x_str = NULL;

    for (size_t i = 0; i < world->args->width; i++) {
        x_str = int_to_str(i);
        if (!x_str)
            continue;
        mct_row(world, gui, fd, x_str);
        free(x_str);
    }
}
