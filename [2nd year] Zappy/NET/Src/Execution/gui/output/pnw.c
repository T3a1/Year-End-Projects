/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** pnw
*/

#include "command.h"
#include "team.h"

void pnw(__attribute__((unused)) world_t *world, ai_t *ai, int fd,
    __attribute__((unused)) char *arg)
{
    logger_client(fd, "pnw #%d %d %d %d %d %s",
        ai->id, ai->position->x, ai->position->y, ai->direction,
        ai->level, ai->team->name);
}
