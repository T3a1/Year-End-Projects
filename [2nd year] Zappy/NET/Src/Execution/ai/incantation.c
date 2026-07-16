/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** incantation
*/

#include "command.h"
#include "team.h"
#include "map.h"
#include "incantation.h"

static bool enough_resources(tile_t *tile, int level)
{
    for (int i = 0; i < RESOURCES_COUNT; i++) {
        if ((size_t) incantation_requirements[level - 1].resources[i] >
            tile->content->resources[i]) {
            logger_server("Not enough resources for incantation at level %d",
                WARNING, level);
            return false;
        }
    }
    return true;
}

static bool enough_players(tile_t *tile, size_t level)
{
    size_t count = 0;

    for (player_t *player = tile->content->players;
        player; player = player->next)
        if (player->ai->level == level)
            count++;
    logger_server("Found %zu players at level %zu, need %zu for incantation",
        OK, count, level, incantation_requirements[level - 1].nb_players);
    return count >= incantation_requirements[level - 1].nb_players;
}

static void level_up_players(world_t *world, tile_t *tile, size_t level)
{
    int fd = -1;

    for (player_t *player = tile->content->players;
        player; player = player->next)
        if (player->ai->level == level) {
            player->ai->level++;
            player->ai->is_incanting = false;
            player->ai->incant_tile = NULL;
            fd = get_fd_from_connections(*world->connections, player->ai);
            logger_client(fd, "Current level: %zu", player->ai->level);
            send_to_guis(world->gui, "plv #%d %d", player->ai->id,
                player->ai->level);
            logger_server("Player %zu leveled up to level %zu",
                OK, player->ai->id, player->ai->level);
        }
}

static void consume_resources(tile_t *tile, size_t level)
{
    for (int i = 0; i < RESOURCES_COUNT; i++)
        tile->content->resources[i] -=
            incantation_requirements[level - 1].resources[i];
}

static void set_incantation_state(world_t *world, tile_t *tile, size_t level)
{
    int fd = -1;

    for (player_t *player = tile->content->players;
        player; player = player->next) {
        if (player->ai->level != level)
            continue;
        player->ai->is_incanting = true;
        player->ai->incant_tile = tile;
        fd = get_fd_from_connections(*world->connections, player->ai);
        logger_client(fd, "Elevation underway");
    }
}

static void remove_incantation_state(world_t *world, tile_t *tile, size_t level)
{
    int fd = -1;

    for (player_t *player = tile->content->players;
        player; player = player->next) {
        if (player->ai->level != level)
            continue;
        player->ai->is_incanting = false;
        player->ai->incant_tile = NULL;
        fd = get_fd_from_connections(*world->connections, player->ai);
        logger_client(fd, "ko");
    }
}

bool verify_incantation(world_t *world, command_t *command)
{
    ai_t *ai = command->player;
    tile_t *tile = ai->position;
    size_t level = ai->level;

    if (level < 1 || level >= 8) {
        logger_server("Invalid level %zu for incantation", WARNING, level);
        return false;
    }
    if (ai->is_incanting && ai->position != ai->incant_tile) {
        return false;
    }
    if (!enough_resources(tile, level) || !enough_players(tile, level))
        return false;
    if (!command->player->is_incanting)
        set_incantation_state(world, tile, level);
    return true;
}

void incantation(world_t *world, command_t *command)
{
    if (!verify_incantation(world, command)) {
        remove_incantation_state(world, command->player->position,
            command->player->level);
        for (gui_t *tmp = world->gui; tmp; tmp = tmp->next) {
            logger_client(tmp->fd, "pie %d %d %d",
                command->player->position->x, command->player->position->y, 0);
        }
        return;
    }
    consume_resources(command->player->position, command->player->level);
    level_up_players(world, command->player->position, command->player->level);
    for (gui_t *tmp = world->gui; tmp; tmp = tmp->next)
        logger_client(tmp->fd, "pie %d %d %d",
            command->player->position->x, command->player->position->y, 1);
}
