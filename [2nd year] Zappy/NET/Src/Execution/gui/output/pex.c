/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** pex
*/

#include "command.h"
#include "team.h"

void pex(__attribute__((unused)) world_t *world, ai_t *ai, int fd,
    __attribute__((unused)) char *arg)
{
    logger_client(fd, "pex #%d", ai->id);
}
