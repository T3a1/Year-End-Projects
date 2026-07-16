/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** eject
*/

#include "command.h"
#include "team.h"
#include "map.h"
#include "server.h"

static void communicate_eject(ai_t *ejected, ai_t *ejector, world_t *world)
{
    int_pair_t vector;
    int fd;
    int dir;

    fd = get_fd_from_connections(*world->connections, ejected);
    if (fd == -1) {
        logger_server("eject: no fd for ai %zu", WARNING, ejected->id);
        return;
    }
    vector = torus_shortest_vector(world,
        &(pos2i_t){ejector->position->x, ejector->position->y},
        &(pos2i_t){ejected->position->x, ejected->position->y});
    dir = get_dir_from_vector(&vector, ejector->direction);
    logger_client(fd, "eject: %d", dir);
    send_to_guis(world->gui, "pex #%zu\nppo #%zu %d %d %d", ejected->id,
        ejected->id, ejected->position->x,
        ejected->position->y, ejected->direction);
}

static void move_ai_to_target(player_t *node, tile_t *src, tile_t *dst)
{
    node = pop_ai_from_list(&src->content->players, node->ai);
    if (!node)
        return;
    node->ai->position = dst;
    node->next = dst->content->players;
    dst->content->players = node;
}

static void move_ais(ai_t *ejector, tile_t *src, tile_t *dst, world_t *world)
{
    player_t *tmp = src->content->players;
    player_t *next = NULL;

    while (tmp) {
        next = tmp->next;
        if (tmp->ai != ejector && tmp->ai->level > 0) {
            communicate_eject(tmp->ai, ejector, world);
            move_ai_to_target(tmp, src, dst);
        }
        tmp = next;
    }
}

static void destroy_eggs(world_t *world, tile_t *tile)
{
    player_t *tmp = tile->content->players;
    player_t *next = NULL;
    player_t *popped = NULL;

    while (tmp) {
        next = tmp->next;
        if (tmp->ai->egg_type == FORK && tmp->ai->level == 0) {
            logger_server("Destroying egg %zu", OK, tmp->ai->id);
            popped = pop_ai_from_list(&tile->content->players, tmp->ai);
            destroy_ai(world, &tmp->ai);
            free(popped);
        }
        tmp = next;
    }
}

void eject(world_t *world, command_t *command)
{
    tile_t *src = command->player->position;
    tile_t *dst = get_front(src, command->player->direction);

    if (!dst)
        return;
    move_ais(command->player, src, dst, world);
    destroy_eggs(world, src);
    logger_client(command->player_fd, "ok");
}
