/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** destroy
*/

#include "ai.h"
#include "world.h"
#include "command.h"
#include "team.h"
#include "gui.h"

static void relink_command(command_t **head, command_t *curr,
    command_t *prev)
{
    if (prev == NULL) {
        *head = curr->next;
        free_array(curr->arguments);
        free(curr);
        return;
    }
    prev->next = curr->next;
    free_array(curr->arguments);
    free(curr);
}

static void free_local_command(command_t *commands)
{
    command_t *to_delete = commands;
    command_t *next = NULL;

    while (to_delete) {
        next = to_delete->next;
        free_array(to_delete->arguments);
        free(to_delete);
        to_delete = next;
    }
}

static void remove_global_commands(command_t **commands, ai_t *dead_ai)
{
    command_t *curr_comm = *commands;
    command_t *prev_comm = NULL;
    command_t *next_comm = NULL;

    while (curr_comm) {
        next_comm = curr_comm->next;
        if (curr_comm->player == dead_ai)
            relink_command(commands, curr_comm, prev_comm);
        else
            prev_comm = curr_comm;
        curr_comm = next_comm;
    }
}

static void relink_ai(ai_t **head, ai_t *curr, ai_t *prev)
{
    if (prev == NULL)
        *head = curr->next;
    else
        prev->next = curr->next;
    free_local_command(curr->local_commands);
    free(curr);
}

static void remove_from_team(ai_t *ai)
{
    player_t *node = pop_ai_from_list(&ai->team->players, ai);

    if (node) {
        ai->team->free_slots++;
        free(node);
    }
}

static void relink_player(tile_t *tile, player_t *prev, player_t *curr)
{
    if (prev == NULL)
        tile->content->players = curr->next;
    else
        prev->next = curr->next;
    free(curr);
}

static void remove_from_tile(ai_t *ai)
{
    tile_t *tile = NULL;
    player_t *curr = NULL;
    player_t *prev = NULL;

    if (!ai->position || !ai->position->content)
        return;
    tile = ai->position;
    curr = tile->content->players;
    while (curr) {
        if (curr->ai == ai)
            return relink_player(tile, prev, curr);
        prev = curr;
        curr = curr->next;
    }
}

static void remove_from_annuary(ai_t **head, ai_t *ai)
{
    ai_t *curr = *head;
    ai_t *prev = NULL;

    while (curr) {
        if (curr == ai) {
            relink_ai(head, curr, prev);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void destroy_ai(world_t *world, ai_t **ai)
{
    if (!*ai)
        return;
    send_to_guis(world->gui, "pdi #%zu", (*ai)->id);
    remove_global_commands(&world->command_queue, *ai);
    remove_from_team(*ai);
    remove_from_tile(*ai);
    remove_from_annuary(&world->ais, *ai);
    *ai = NULL;
}

void destroy_ais(ai_t *ai)
{
    ai_t *current = NULL;
    ai_t *next = NULL;
    command_t *tmp_command = NULL;
    command_t *next_command = NULL;

    if (ai == NULL)
        return;
    current = ai;
    while (current != NULL) {
        next = current->next;
        tmp_command = current->local_commands;
        while (tmp_command) {
            next_command = tmp_command->next;
            free_array(tmp_command->arguments);
            free(tmp_command);
            tmp_command = next_command;
        }
        free(current);
        current = next;
    }
}
