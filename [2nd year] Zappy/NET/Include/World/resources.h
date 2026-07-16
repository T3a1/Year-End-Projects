/*
** EPITECH PROJECT, 2026
** Visual Studio Live Share (Workspace)
** File description:
** resources
*/

#ifndef RESOURCES_H_
    #define RESOURCES_H_

    #define RESOURCES_COUNT 7

    #include "world.h"

typedef enum {
    FOOD,
    LINEMATE,
    DERAUMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME
} resource_t;

static const double RESOURCES_DENSITIES[] = {
    0.5, // FOOD
    0.3, // LINEMATE
    0.15, // DERAUMERE
    0.1, // SIBUR
    0.1, // MENDIANE
    0.08, // PHIRAS
    0.05 // THYSTAME
};

static const resource_t RESOURCES[] = {
    FOOD,
    LINEMATE,
    DERAUMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME
};

/**
 * @brief Spawns missing resources across the map.
 *
 * Builds an array of pointers to every tile of the map, then, for
 * each resource type, shuffles that array and places resources on
 * random tiles via spawn_one_resource until the target quantity
 * (computed from the map's surface and the resource's density) is
 * reached.
 *
 * @param world Pointer to the world_t structure holding the map,
 * the resource densities and the current resource counts.
 *
 * @return true if the resources were successfully spawned, false if
 * the tile array allocation failed.
 */
bool spawn_resources(world_t *world);

#endif /* !RESOURCES_H_ */
