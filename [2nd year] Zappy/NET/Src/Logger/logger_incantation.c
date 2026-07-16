/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** logger_client
*/

#include "logger.h"
#include "gui.h"
#include "map.h"
#include "ai.h"

static size_t get_player_count(player_t *players, size_t level)
{
    size_t count = 0;

    for (; players; players = players->next)
        if (players->ai->level == level)
            count++;
    return count;
}

static size_t get_command_size(size_t nb_player)
{
    return HEADER_SIZE + (nb_player * PLAYER_ENTRY_SIZE) + 2;
}

static void append_players(char *buffer, player_t *players, size_t level)
{
    char id_str[PLAYER_ENTRY_SIZE];

    for (; players; players = players->next) {
        if (players->ai->level != level)
            continue;
        snprintf(id_str, sizeof(id_str), " #%zu", players->ai->id);
        strcat(buffer, id_str);
    }
}

static char *build_incantation_message(tile_t *tile, size_t level)
{
    player_t *players = tile->content->players;
    size_t player_count = get_player_count(players, level);
    char *buffer = calloc(get_command_size(player_count), sizeof(char));

    if (!buffer)
        return NULL;
    sprintf(buffer, "pic %d %d %zu", tile->x, tile->y, level);
    append_players(buffer, players, level);
    return buffer;
}

void logger_incantation(gui_t *guis, tile_t *tile, size_t level)
{
    gui_t *current_gui = guis;
    char *buffer = build_incantation_message(tile, level);

    if (!buffer)
        return;
    for (; current_gui; current_gui = current_gui->next)
        logger_client(current_gui->fd, buffer);
    free(buffer);
}
