/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** pic
*/

#include "command.h"
#include "team.h"

void pic(__attribute__((unused)) world_t *world, ai_t *ai, int fd,
    __attribute__((unused)) char *arg)
{
    logger_client(fd, "pic #%d", ai->id);
}
