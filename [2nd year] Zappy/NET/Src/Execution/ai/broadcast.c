/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** broadcast
*/

#include "map.h"
#include "world.h"
#include "ai.h"
#include "command.h"

bool check_broadcast_command(char **command)
{
    int nb_args = count_args(command);

    if (nb_args >= 1 && nb_args <= 1)
        return true;
    logger_server("Invalid number of arguments for Broadcast command", WARNING);
    return false;
}

static void broadcast_to_ai(world_t *world, command_t *command,
    pos2i_t *start)
{
    int_pair_t vector;
    pos2i_t end;
    connection_t *conn = *world->connections;
    ai_t *ai;

    while (conn) {
        ai = conn->cli.ai;
        if (conn->type == AI && ai && ai->level > 0
            && ai != command->player) {
            end.x = ai->position->x;
            end.y = ai->position->y;
            vector = torus_shortest_vector(world, start, &end);
            logger_client(conn->fd, "message %d, %s",
                get_dir_from_vector(&vector, ai->direction),
                command->arguments[1]);
        }
        conn = conn->next;
    }
}

static void broadcast_to_gui(world_t *world, command_t *command,
    char **args)
{
    connection_t *conn = *world->connections;

    while (conn) {
        if (conn->type == GUI)
            pbc(world, command->player, conn->fd, args[0]);
        conn = conn->next;
    }
}

void broadcast(world_t *world, command_t *command)
{
    pos2i_t start;
    char *args[] = {command->arguments[1], NULL};

    start.x = command->player->position->x;
    start.y = command->player->position->y;
    broadcast_to_ai(world, command, &start);
    broadcast_to_gui(world, command, args);
    logger_client(command->player_fd, "ok");
}
