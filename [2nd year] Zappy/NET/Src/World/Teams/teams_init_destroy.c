/*
** EPITECH PROJECT, 2026
** Visual Studio Live Share (Workspace)
** File description:
** init_destroy
*/

#include "team.h"

void free_teams(team_t *teams)
{
    team_t *tmp = teams;
    team_t *next = NULL;
    player_t *p = NULL;
    player_t *pnext = NULL;

    while (tmp) {
        next = tmp->next;
        free(tmp->name);
        for (p = tmp->players; p; p = pnext) {
            pnext = p->next;
            free(p);
        }
        free(tmp);
        tmp = next;
    }
}

static void add_team(team_t **head, team_t *new_team)
{
    new_team->next = *head;
    *head = new_team;
}

static bool alloc_team_fields(team_t *team, world_t *world, size_t i)
{
    team->name = strdup(world->args->team_names[i]);
    if (!team->name)
        return false;
    team->players = NULL;
    return true;
}

static team_t *create_team(world_t *world, size_t i)
{
    team_t *new_team = calloc(1, sizeof(team_t));

    if (!new_team)
        return NULL;
    if (!alloc_team_fields(new_team, world, i)) {
        free(new_team);
        return NULL;
    }
    new_team->free_slots = world->args->eggs_per_team;
    new_team->next = NULL;
    return new_team;
}

bool init_teams(world_t *world)
{
    team_t *head = NULL;
    team_t *new_team = NULL;

    for (size_t i = 0; world->args->team_names[i] != NULL; i++) {
        new_team = create_team(world, i);
        if (!new_team) {
            free_teams(head);
            return false;
        }
        add_team(&head, new_team);
    }
    world->teams = head;
    return true;
}
