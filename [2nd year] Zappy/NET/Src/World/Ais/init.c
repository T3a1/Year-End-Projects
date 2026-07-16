/*
** EPITECH PROJECT, 2026
** Visual Studio Live Share (Workspace)
** File description:
** init_destroy
*/

#include "ai.h"
#include "world.h"
#include "team.h"
#include "map.h"
#include "logger.h"
#include "command.h"

static size_t next_id(world_t *world)
{
    size_t id = world->next_id;

    world->next_id++;
    return id;
}

static void add_egg_to_tile(ai_t *egg)
{
    tile_t *tile = egg->position;
    player_t *node = NULL;

    if (!tile || !tile->content)
        return;
    node = calloc(1, sizeof(player_t));
    if (!node) {
        logger_server("Failed to allocate player node", MAJOR);
        return;
    }
    node->ai = egg;
    node->next = tile->content->players;
    tile->content->players = node;
}

static void add_egg_to_team(team_t *team, ai_t *egg)
{
    player_t *node = calloc(1, sizeof(player_t));

    if (!node) {
        logger_server("Failed to allocate player node for team", MAJOR);
        return;
    }
    node->ai = egg;
    node->next = team->players;
    team->players = node;
}

static void init_egg(ai_t *egg, team_t *team, size_t id)
{
    egg->id = id;
    egg->direction = (rand() % 4) + 1;
    egg->level = 0;
    egg->id_owner = -1;
    egg->team = team;
    egg->local_commands = NULL;
    egg->last_check = -1;
    egg->egg_type = INITIAL;
    memset(egg->inventory, 0, sizeof(egg->inventory));
    egg->inventory[FOOD] = 10;
    egg->incant_tile = NULL;
    egg->is_incanting = false;
}

static ai_t *create_egg(team_t *team, size_t id,
    tile_t *position)
{
    ai_t *egg = calloc(1, sizeof(ai_t));

    if (!egg) {
        logger_server("Failed to allocate memory for egg", MAJOR);
        return NULL;
    }
    init_egg(egg, team, id);
    egg->position = position;
    return egg;
}

static bool spawn_team_eggs(world_t *world, team_t *team)
{
    ai_t *egg = NULL;

    for (size_t i = 0; i < world->args->eggs_per_team; i++) {
        egg = create_egg(team, next_id(world), get_random_tile(world));
        if (!egg)
            return false;
        egg->next = world->ais;
        world->ais = egg;
        add_egg_to_tile(egg);
        add_egg_to_team(team, egg);
    }
    world->next_id++;
    return true;
}

bool spawn_eggs(world_t *world)
{
    for (team_t *team = world->teams; team != NULL; team = team->next) {
        if (!spawn_team_eggs(world, team))
            return false;
    }
    return true;
}

ai_t *spawn_fork_egg(world_t *world, team_t *team, ai_t *parent_ai)
{
    ai_t *new_egg = create_egg(team,
        parent_ai->id, parent_ai->position);

    if (!new_egg)
        return NULL;
    new_egg->egg_type = FORK;
    new_egg->id = next_id(world);
    new_egg->id_owner = parent_ai->id;
    new_egg->next = world->ais;
    world->ais = new_egg;
    add_egg_to_tile(new_egg);
    add_egg_to_team(team, new_egg);
    return new_egg;
}
