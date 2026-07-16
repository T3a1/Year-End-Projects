/*
** EPITECH PROJECT, 2026
** Visual Studio Live Share (Workspace)
** File description:
** is_valid_team
*/

#include "world.h"
#include "team.h"
#include "logger.h"

static bool spot_available(team_t *team)
{
    if (team->free_slots > 0) {
        return true;
    }
    return false;
}

bool is_available_team(world_t *world, char *team_name)
{
    team_t *curr_team = world->teams;

    while (curr_team) {
        if (strcmp(curr_team->name, team_name) == 0 &&
            spot_available(curr_team)) {
            return true;
        }
        curr_team = curr_team->next;
    }
    logger_server("No available team found for name: %s", WARNING, team_name);
    return false;
}
