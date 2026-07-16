/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** pin
*/

#include "command.h"
#include "map.h"
#include "parser.h"

void pin(__attribute__((unused)) world_t *world,
    __attribute__((unused)) gui_t *gui, char **args, int fd)
{
    int player_num = parse_player_number(args[1]);

    if (player_num < 0) {
        logger_client(fd, "sbp");
        return;
    }
    for (ai_t *tmp = world->ais; tmp != NULL; tmp = tmp->next) {
        if ((int) tmp->id == player_num && tmp->level > 0) {
            logger_client(fd, "pin #%d %d %d %d %d %d %d %d %d %d", player_num,
                tmp->position->x, tmp->position->y, tmp->inventory[0],
                tmp->inventory[1], tmp->inventory[2], tmp->inventory[3],
                tmp->inventory[4], tmp->inventory[5], tmp->inventory[6]
            );
            return;
        }
    }
    logger_client(fd, "sbp");
}
