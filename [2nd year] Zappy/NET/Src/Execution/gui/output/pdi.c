/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** pdi
*/

#include "command.h"
#include "team.h"

void pdi(__attribute__((unused)) world_t *world, ai_t *ai, int fd,
    __attribute__((unused)) char *arg)
{
    logger_client(fd, "pdi #%d", ai->id);
}
