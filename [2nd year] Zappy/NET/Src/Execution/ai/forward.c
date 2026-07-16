/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** forward
*/

#include "command.h"

static player_t *pop_head(player_t **head)
{
    player_t *node = *head;

    *head = (*head)->next;
    node->next = NULL;
    return node;
}

static player_t *pop_inner(player_t *current, ai_t *ai_to_remove)
{
    player_t *node = NULL;

    while (current->next) {
        if (current->next->ai == ai_to_remove) {
            node = current->next;
            current->next = node->next;
            node->next = NULL;
            return node;
        }
        current = current->next;
    }
    return NULL;
}

player_t *pop_ai_from_list(player_t **head, ai_t *ai_to_remove)
{
    if (!*head)
        return NULL;
    if ((*head)->ai == ai_to_remove)
        return pop_head(head);
    return pop_inner(*head, ai_to_remove);
}

void forward(__attribute__((unused)) world_t *world, command_t *command)
{
    tile_t *current_tile = command->player->position;
    tile_t *target_tile = get_front(current_tile, command->player->direction);
    player_t *node = pop_ai_from_list(
        &current_tile->content->players, command->player);

    if (!node || !target_tile)
        return logger_client(command->player_fd, "ko");
    command->player->position = target_tile;
    node->next = target_tile->content->players;
    target_tile->content->players = node;
    logger_client(command->player_fd, "ok");
    for (gui_t *tmp = world->gui; tmp; tmp = tmp->next)
        logger_client(tmp->fd, "ppo %zu %d %d %d", command->player->id,
            command->player->position->x, command->player->position->y,
            command->player->direction);
}
