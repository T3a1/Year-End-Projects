/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** pdr
*/

#include "command.h"
#include "utils.h"
#include "logger.h"
#include "team.h"

void pdr(__attribute__((unused)) world_t *world, ai_t *ai, int fd, char *arg)
{
    int value = 0;

    if (!get_integer(arg, &value)) {
        logger_client(fd, "sbp");
        return;
    }
    logger_client(fd, "pdr #%d %d", ai->id, value);
}
