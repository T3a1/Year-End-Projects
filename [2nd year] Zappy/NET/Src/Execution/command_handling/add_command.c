/*
** EPITECH PROJECT, 2026
** Visual Studio Live Share (Workspace)
** File description:
** add_command
*/

#include "command.h"

static bool already_has_global_command(command_t *head, ai_t *player)
{
    for (command_t *tmp = head; tmp; tmp = tmp->next) {
        if (tmp->player == player)
            return true;
    }
    return false;
}

static void insert_global_command(command_t **head,
    command_t *new_command)
{
    command_t *current = *head;
    command_t *prev = NULL;

    while (current != NULL
        && current->remaining_time <= new_command->remaining_time) {
        prev = current;
        current = current->next;
    }
    new_command->next = current;
    if (prev == NULL)
        *head = new_command;
    else
        prev->next = new_command;
}

static void insert_local_command(command_t **head,
    command_t *new_command)
{
    command_t *current = *head;

    new_command->next = NULL;
    if (current == NULL) {
        *head = new_command;
        return;
    }
    for (; current->next; current = current->next);
    current->next = new_command;
}

void add_command(world_t *world, command_t *new_command)
{
    if (already_has_global_command(world->command_queue, new_command->player))
        insert_local_command(&new_command->player->local_commands, new_command);
    else
        add_global_command(world, new_command);
}

static bool already_incanting_here(ai_t *player)
{
    return player->is_incanting && player->position == player->incant_tile;
}

static bool pre_incantation_check(world_t *world, command_t *new_command)
{
    if (already_incanting_here(new_command->player))
        return false;
    if (!verify_incantation(world, new_command)) {
        logger_client(new_command->player_fd, "ko");
        return false;
    }
    logger_incantation(world->gui, new_command->player->position,
        new_command->player->level);
    return true;
}

void add_global_command(world_t *world, command_t *new_command)
{
    command_t **head = &world->command_queue;

    if (!head || !new_command)
        return;
    if (strcmp(new_command->arguments[0], "Incantation") == 0) {
        if (!pre_incantation_check(world, new_command)) {
            free_array(new_command->arguments);
            free(new_command);
            return;
        }
    }
    insert_global_command(head, new_command);
    if (world->old_time_commands == 0)
        world->old_time_commands = get_time_ms();
    world->next_command_time = (*head == NULL) ? -1 : (*head)->remaining_time;
}
