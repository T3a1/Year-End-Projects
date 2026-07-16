/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** pie
*/

#include "command.h"
#include "team.h"

void pie(__attribute__((unused)) world_t *world, ai_t *ai, int fd, char *arg)
{
    logger_client(fd, "pie %d %d %d",
        ai->position->x, ai->position->y,
        (!strcmp(arg, "true")) ? 1 : 0);
}
