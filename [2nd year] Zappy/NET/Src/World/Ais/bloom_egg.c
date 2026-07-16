/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** bloom_egg
*/

#include "ai.h"
#include "logger.h"
#include "team.h"

static void init_ai(ai_t *ai)
{
    ai->level = 1;
    ai->team->free_slots--;
}

static ai_t *find_free_egg(team_t *team)
{
    for (player_t *p = team->players; p; p = p->next) {
        if (p->ai->level == 0)
            return p->ai;
    }
    return NULL;
}

ai_t *bloom_egg(world_t *world, char *team_name)
{
    ai_t *egg = NULL;

    for (team_t *team = world->teams; team != NULL; team = team->next) {
        if (strcmp(team->name, team_name) == 0) {
            egg = find_free_egg(team);
            break;
        }
    }
    if (egg == NULL) {
        logger_server("No free egg slot for new AI", MAJOR);
        return NULL;
    }
    init_ai(egg);
    return egg;
}
