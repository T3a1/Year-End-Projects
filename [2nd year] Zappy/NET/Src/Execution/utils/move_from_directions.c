/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** move_from_directions
*/

#include "command.h"
#include "map.h"

tile_t *get_front(tile_t *t, direction_t dir)
{
    switch (dir) {
        case NORTH:
            return t->north;
        case SOUTH:
            return t->south;
        case EAST:
            return t->east;
        case WEST:
            return t->west;
    }
    return NULL;
}

tile_t *get_right(tile_t *t, direction_t dir)
{
    switch (dir) {
        case NORTH:
            return t->east;
        case SOUTH:
            return t->west;
        case EAST:
            return t->south;
        case WEST:
            return t->north;
    }
    return NULL;
}

tile_t *get_left(tile_t *t, direction_t dir)
{
    switch (dir) {
        case NORTH:
            return t->west;
        case SOUTH:
            return t->east;
        case EAST:
            return t->north;
        case WEST:
            return t->south;
    }
    return NULL;
}
