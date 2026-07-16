/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** win
*/

#include "ai.h"
#include "team.h"
#include "logger.h"
#include "gui.h"

static unsigned int count_max_level(player_t *players)
{
    unsigned int nb_win = 0;

    for (player_t *p = players; p; p = p->next) {
        if (p->ai->level == 8)
            nb_win++;
    }
    return nb_win;
}

static bool team_wins(team_t *team, gui_t *gui)
{
    if (count_max_level(team->players) < 6)
        return false;
    for (gui_t *tmp = gui; tmp; tmp = tmp->next) {
        logger_client(tmp->fd, "seg %s\n", team->name);
    }
    logger_server("Team %s wins the game!", OK, team->name);
    return true;
}

bool win(world_t *world)
{
    team_t *team = world->teams;

    while (team) {
        if (team_wins(team, world->gui))
            return true;
        team = team->next;
    }
    return false;
}
