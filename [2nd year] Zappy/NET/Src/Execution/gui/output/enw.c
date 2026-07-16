/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** enw
*/

#include "command.h"

void enw(__attribute__((unused)) world_t *world,
    __attribute__((unused)) gui_t *gui,
    __attribute__((unused)) char **args, int fd)
{
    ai_t *ais = world->ais;
    int id_owner = 0;

    while (ais) {
        if (ais->egg_type == INITIAL)
            id_owner = -1;
        else
            id_owner = ais->id_owner;
        logger_client(fd, "enw #%d #%d %d %d", ais->id, id_owner,
            ais->position->x, ais->position->y);
        ais = ais->next;
    }
}
