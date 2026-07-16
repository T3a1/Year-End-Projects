/*
** EPITECH PROJECT, 2026
** Visual Studio Live Share (Workspace)
** File description:
** refresh_commands
*/

#include "command.h"
#include "utils.h"
#include <time.h>

static command_t *remove_current(command_t **head,
    command_t **prev, command_t *current)
{
    command_t *next = current->next;

    if (*prev == NULL)
        *head = next;
    else
        (*prev)->next = next;
    free_array(current->arguments);
    free(current);
    return next;
}

static void re_add_refill(world_t *world)
{
    char **args = calloc(2, sizeof(char *));
    command_t *new_command = NULL;

    if (!args)
        return;
    args[0] = strdup("REFILL");
    if (!args[0]) {
        free(args);
        return;
    }
    args[1] = NULL;
    new_command = create_command(args, NULL, 20000 / world->args->freq, -1);
    if (!new_command) {
        free(args[0]);
        free(args);
        return;
    }
    add_global_command(world, new_command);
}

static command_t *pop_local_command(command_t **local_queue)
{
    command_t *removed = *local_queue;

    if (removed == NULL)
        return NULL;
    *local_queue = removed->next;
    return removed;
}

static void queue_promotion(command_t **to_promote, command_t *new_node)
{
    new_node->next = *to_promote;
    *to_promote = new_node;
}

static void promote_local_command(ai_t *player, command_t **to_promote)
{
    command_t *next_local;

    if (player == NULL)
        return;
    next_local = pop_local_command(&player->local_commands);
    if (next_local != NULL)
        queue_promotion(to_promote, next_local);
}

static void process_command(server_t *server, command_t **prev,
    command_t **current, command_t **to_promote)
{
    command_t **head = &server->world->command_queue;
    bool is_refill = (strcmp((*current)->arguments[0], "REFILL") == 0);
    ai_t *player = (*current)->player;

    execute_command(server, *current);
    if (is_refill) {
        free((*current)->arguments[0]);
        free((*current)->arguments);
        (*current)->arguments = NULL;
    }
    *current = remove_current(head, prev, *current);
    if (is_refill)
        re_add_refill(server->world);
    promote_local_command(player, to_promote);
}

static void promote_pending_commands(world_t *world, command_t *to_promote)
{
    command_t *next;

    while (to_promote != NULL) {
        next = to_promote->next;
        add_global_command(world, to_promote);
        to_promote = next;
    }
}

void refresh_commands(server_t *server)
{
    command_t *current = server->world->command_queue;
    command_t *prev = NULL;
    command_t *to_promote = NULL;
    long current_time = get_time_ms();
    long elapsed = current_time - server->world->old_time_commands;

    while (current != NULL) {
        if (current->remaining_time > elapsed) {
            current->remaining_time -= elapsed;
            prev = current;
            current = current->next;
        } else {
            process_command(server, &prev, &current, &to_promote);
        }
    }
    server->world->old_time_commands = current_time;
    promote_pending_commands(server->world, to_promote);
    server->world->next_command_time =
        (server->world->command_queue == NULL) ? -1 :
        server->world->command_queue->remaining_time;
}
