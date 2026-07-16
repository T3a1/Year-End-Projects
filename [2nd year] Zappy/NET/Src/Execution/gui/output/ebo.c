/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** ebo
*/

#include "command.h"
#include "team.h"

void ebo(__attribute__((unused)) world_t *world, ai_t *ai, int fd,
    __attribute__((unused)) char *arg)
{
    logger_client(fd, "ebo #%d", ai->id);
}
