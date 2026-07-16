/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** refresh_ais
*/

#include "ai.h"
#include "world.h"
#include "logger.h"
#include "connection.h"
#include "command.h"
#include "team.h"
#include <stdio.h>

static bool handle_dead_ai(world_t *world, ai_t *ai, connection_t *conns)
{
    connection_t *cnx = conns;

    while (cnx) {
        if (cnx->cli.ai == ai)
            break;
        cnx = cnx->next;
    }
    if (!cnx) {
        logger_server("Could not find fd for ai %zu", MAJOR, ai->id);
        return true;
    }
    logger_server("AI %zu dies.", INFO, ai->id);
    logger_client(cnx->fd, "dead");
    destroy_ai(world, &cnx->cli.ai);
    cnx->type = UNKNOWN;
    return false;
}

static bool check_life_and_resources(world_t *world, ai_t *ai,
    connection_t *conns, unsigned int due)
{
    if (ai->inventory[FOOD] <= (int) due)
        return handle_dead_ai(world, ai, conns);
    ai->inventory[FOOD] -= due;
    if (world->args->verbose)
        logger_server("AI %zu consumes %d food, still got %d", INFO,
            ai->id, due, ai->inventory[FOOD]);
    send_to_guis(world->gui, "pin #%d %d %d %d %d %d %d %d %d %d",
        ai->id, ai->position->x, ai->position->y, ai->inventory[0],
        ai->inventory[1], ai->inventory[2], ai->inventory[3],
        ai->inventory[4], ai->inventory[5], ai->inventory[6]);
    return true;
}

static unsigned int ai_check_due(ai_t *ai, world_t *world, long curr)
{
    long elapsed_ms = 0;
    long period_ms = 0;

    period_ms = (126 * 1000) / world->args->freq;
    if (ai->last_check == -1)
        elapsed_ms = period_ms + 1;
    else
        elapsed_ms = curr - ai->last_check;
    if (elapsed_ms < period_ms)
        return 0;
    return (unsigned int)(elapsed_ms / period_ms);
}

static void process_ai(world_t *world, ai_t *ai, connection_t *conns,
    long curr)
{
    unsigned int due = 0;

    if (!ai || ai->level == 0)
        return;
    due = ai_check_due(ai, world, curr);
    if (due == 0)
        return;
    if (check_life_and_resources(world, ai, conns, due))
        ai->last_check = curr;
}

void refresh_ais(world_t *world, connection_t *connections)
{
    ai_t *current = world->ais;
    ai_t *next = NULL;
    long curr = get_time_ms();

    while (current) {
        next = current->next;
        process_ai(world, current, connections, curr);
        current = next;
    }
}
