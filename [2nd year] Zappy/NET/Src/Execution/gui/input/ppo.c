/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** ppo
*/

#include "command.h"
#include "parser.h"
#include "logger.h"

void ppo(__attribute__((unused)) world_t *world,
    __attribute__((unused)) gui_t *gui, char **args, int fd)
{
    int player_num = parse_player_number(args[1]);

    if (player_num < 0) {
        logger_client(fd, "sbp");
        return;
    }
    for (ai_t *tmp = world->ais; tmp != NULL; tmp = tmp->next) {
        if ((int) tmp->id == player_num && tmp->level > 0) {
            logger_client(fd, "ppo #%d %d %d %d", player_num,
                tmp->position->x, tmp->position->y, tmp->direction);
            return;
        }
    }
    logger_client(fd, "sbp");
}
