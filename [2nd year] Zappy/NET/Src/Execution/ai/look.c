/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** look
*/
#include "command.h"
#include "world.h"
#include "map.h"
#include "logger.h"

static void trim_trailing_space(char *buffer)
{
    size_t len = strlen(buffer);

    if (len > 0 && buffer[len - 1] == ' ')
        buffer[len - 1] = '\0';
}

static void close_line(char *buffer)
{
    trim_trailing_space(buffer);
    strcat(buffer, "]");
}

static bool add_to_buffer(buf_t *buf, const char *str)
{
    size_t needed = strlen(buf->data) + strlen(str) + 1;
    char *tmp = NULL;

    if (needed > buf->cap) {
        tmp = realloc(buf->data, needed * 2);
        if (!tmp)
            return false;
        buf->data = tmp;
        buf->cap = needed * 2;
    }
    strcat(buf->data, str);
    return true;
}

static size_t get_nb_players_on_tile(tile_t *tile)
{
    size_t count = 0;
    player_t *tmp = NULL;

    if (tile->content == NULL || tile->content->players == NULL)
        return 0;
    tmp = tile->content->players;
    while (tmp) {
        if (tmp->ai->level > 0)
            count++;
        tmp = tmp->next;
    }
    return count;
}

static void add_resources(tile_content_t *content, buf_t *buf)
{
    const char *names[] = {
        "food ", "linemate ", "deraumere ",
        "sibur ", "mendiane ", "phiras ", "thystame "
    };

    for (int r = 0; r < RESOURCES_COUNT; r++)
        for (size_t i = 0; i < content->resources[r]; i++)
            add_to_buffer(buf, names[r]);
}

static void add_tile_content(tile_t *tile, buf_t *buf)
{
    tile_content_t *content = tile->content;
    size_t nb_players = get_nb_players_on_tile(tile);

    if (content == NULL)
        return;
    for (size_t i = 0; i < nb_players; i++)
        add_to_buffer(buf, "player ");
    add_resources(content, buf);
}

static void separate_tile(buf_t *buf)
{
    size_t len = strlen(buf->data);

    if (len > 1 && buf->data[len - 1] != '[') {
        trim_trailing_space(buf->data);
        strcat(buf->data, ", ");
    }
}

static void process_row(ai_t *player, tile_t **tile_it,
    buf_t *buf, unsigned int len)
{
    for (unsigned int j = 0; j < len; j++) {
        separate_tile(buf);
        add_tile_content(*tile_it, buf);
        *tile_it = get_right(*tile_it, player->direction);
    }
}

static tile_t *advance_row(ai_t *player, tile_t *start)
{
    start = get_front(start, player->direction);
    start = get_left(start, player->direction);
    return start;
}

void look(__attribute__((unused)) world_t *world, command_t *command)
{
    unsigned int curr_len = 1;
    ai_t *player = command->player;
    tile_t *start_tile = player->position;
    tile_t *tile_iterator = start_tile;
    buf_t buf = {calloc(BUFFER_SIZE, sizeof(char)), BUFFER_SIZE};

    if (!buf.data)
        return;
    strcat(buf.data, "[");
    for (unsigned int i = 0; i <= player->level; i++) {
        process_row(player, &tile_iterator, &buf, curr_len);
        curr_len += 2;
        start_tile = advance_row(player, start_tile);
        tile_iterator = start_tile;
    }
    close_line(buf.data);
    logger_client(command->player_fd, buf.data);
    free(buf.data);
}
