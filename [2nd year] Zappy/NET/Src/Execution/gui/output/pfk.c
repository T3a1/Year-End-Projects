/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** pfk
*/

#include "command.h"
#include "team.h"

void pfk(__attribute__((unused)) world_t *world, ai_t *ai, int fd,
    __attribute__((unused)) char *arg)
{
    logger_client(fd, "pfk #%d", ai->id);
}
