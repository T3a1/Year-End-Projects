/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** plv
*/

#include "command.h"
#include "utils.h"
#include "parser.h"
#include "logger.h"

void plv(__attribute__((unused)) world_t *world,
    __attribute__((unused)) gui_t *gui, char **args, int fd)
{
    int player_num = parse_player_number(args[1]);

    if (player_num < 0) {
        logger_client(fd, "sbp");
        return;
    }
    for (ai_t *tmp = world->ais; tmp; tmp = tmp->next) {
        if ((int)tmp->id == player_num && tmp->level > 0) {
            logger_client(fd, "plv #%d %d", player_num, tmp->level);
            return;
        }
    }
    logger_client(fd, "sbp");
}
