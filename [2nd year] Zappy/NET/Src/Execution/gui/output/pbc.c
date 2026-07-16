/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** pbc
*/

#include "command.h"
#include "team.h"

void pbc(__attribute__((unused)) world_t *world, ai_t *ai, int fd,
    __attribute__((unused)) char *arg)
{
    logger_client(fd, "pbc #%d %s", ai->id, arg);
}
