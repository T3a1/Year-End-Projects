/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** bct
*/

#include "command.h"

void bct(__attribute__((unused)) world_t *world,
    __attribute__((unused)) gui_t *gui, char **args, int fd)
{
    int x = 0;
    int y = 0;
    tile_t *tile = NULL;

    if (!get_integer(args[1], &x) || !get_integer(args[2], &y)) {
        logger_client(fd, "sbp");
        return;
    }
    tile = get_tile(world, x, y);
    logger_client(fd, "bct %d %d %d %d %d %d %d %d %d", x, y,
        tile->content->resources[FOOD],
        tile->content->resources[LINEMATE], tile->content->resources[DERAUMERE],
        tile->content->resources[SIBUR], tile->content->resources[MENDIANE],
        tile->content->resources[PHIRAS], tile->content->resources[THYSTAME]);
}
