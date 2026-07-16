/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** pgt
*/

#include "command.h"
#include "team.h"

void pgt(__attribute__((unused)) world_t *world, ai_t *ai, int fd,
    __attribute__((unused)) char *arg)
{
    int value = 0;

    if (!get_integer(arg, &value)) {
        logger_client(fd, "sbp");
        return;
    }
    logger_client(fd, "pgt #%d %d", ai->id, value);
}
