/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** graphic
*/

#include "command.h"
#include "gui.h"
#include "world.h"

void graphic(world_t *world, gui_t *gui, char **args, int fd)
{
    msz(world, gui, args, fd);
    sgt(world, gui, args, fd);
    mct(world, gui, args, fd);
    tna(world, gui, args, fd);
    enw(world, gui, args, fd);
}
