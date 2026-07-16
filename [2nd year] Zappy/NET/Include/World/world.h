/*
** EPITECH PROJECT, 2026
** Visual Studio Live Share (Workspace)
** File description:
** world
*/

#ifndef WORLD_H_
    #define WORLD_H_

    #include <time.h>
    #include <stdbool.h>
    #include <unistd.h>
    #include <stdlib.h>

    #include "args.h"

typedef struct resource_stocks resource_stocks_t;
typedef struct connection_s connection_t;
typedef struct command_s command_t;
typedef struct tile tile_t;
typedef struct team team_t;
typedef struct gui gui_t;
typedef struct ai ai_t;

typedef struct world {
    tile_t *grid;
    args_t *args;
    ai_t *ais;
    gui_t *gui;
    team_t *teams;
    command_t *command_queue;
    long old_time_commands;
    size_t resource_counts[7];
    int next_command_time;
    size_t next_id;
    connection_t **connections;
} world_t;

/**
 * @brief Allocates the tile grid and the world structure.
 *
 * Allocates an array of size tiles for the map grid and a single
 * world_t structure, links the grid into the world, and frees both
 * on failure.
 *
 * @param size Number of tiles to allocate for the grid (width *
 * height).
 * @param grid Output pointer set to the newly allocated tile array.
 * @param world Output pointer set to the newly allocated world_t
 * structure.
 *
 * @return true if both allocations succeeded, false otherwise (in
 * which case any partially allocated memory is freed).
 */
bool alloc_grid_buffers(size_t size, tile_t **grid, world_t **world);

/**
 * @brief Builds and initializes the world's tile grid.
 *
 * Initializes every tile of the world's grid, row by row, via
 * init_row: setting its coordinates, linking it to its four
 * toroidal neighbors, and allocating its content.
 *
 * @param world Pointer to the world_t structure whose grid should
 * be built.
 *
 * @return true if every tile was successfully initialized, false if
 * any tile's content allocation failed.
 */
bool build_grid(world_t *world);

/**
 * @brief Returns the tile at the given coordinates.
 *
 * Checks that the given x and y coordinates lie within the map's
 * bounds, then returns a pointer to the corresponding tile in the
 * grid.
 *
 * @param world Pointer to the world_t structure holding the grid
 * and its dimensions.
 * @param x X coordinate of the tile to retrieve.
 * @param y Y coordinate of the tile to retrieve.
 *
 * @return A pointer to the matching tile_t structure, or NULL if x
 * or y is out of bounds.
 */
tile_t *get_tile(world_t *world, size_t x, size_t y);

/**
 * @brief Returns a random tile from the world's grid.
 *
 * Picks random x and y coordinates within the map's width and
 * height and returns a pointer to the corresponding tile.
 *
 * @param world Pointer to the world_t structure holding the grid
 * and its dimensions.
 *
 * @return A pointer to a randomly chosen tile_t in the grid.
 */
tile_t *get_random_tile(world_t *world);

/**
 * @brief Creates and initializes the whole game world.
 *
 * Validates that the map dimensions are large enough (at least 10
 * by 10), allocates the grid and the world_t structure, builds the
 * tile grid, then sets up the world's remaining state (recurring
 * REFILL command, resources, teams and initial eggs) via
 * setup_world.
 *
 * @param args Pointer to the parsed server arguments, holding the
 * map dimensions, teams and other configuration values.
 *
 * @return A pointer to the newly created and initialized world_t
 * structure on success, or NULL if the dimensions are invalid or
 * any allocation/initialization step failed.
 */
world_t *create_world(args_t *args);

/**
 * @brief Frees the world structure and all its components.
 *
 * Frees the team list, the content of every tile of the grid, the
 * pending command queue, the GUI list and the AI list, then frees
 * the grid itself and the world_t structure. Does nothing if the
 * given pointer is NULL.
 *
 * @param world Pointer to the world_t structure to destroy.
 *
 * @return None (void).
 */
void destroy_world(world_t *trantor);

/**
 * @brief Refreshes the food consumption state of every AI.
 *
 * Iterates over every AI in the world and, for each one, checks
 * how many food consumption periods are due since its last check
 * via process_ai. Living AIs that have run out of food are killed
 * (their connection is reset, a "dead" message is sent and they are
 * destroyed), while surviving AIs have their food consumed and
 * their state broadcast to every GUI client.
 *
 * @param world Pointer to the world_t structure holding the AI
 * list.
 * @param connections Pointer to the head of the connection linked
 * list, used to find the file descriptor associated with a dying
 * AI.
 *
 * @return None (void).
 */
void refresh_ais(world_t *world, connection_t *connections);

/**
 * @brief Returns the current monotonic time in milliseconds.
 *
 * Retrieves the current time from the CLOCK_MONOTONIC clock and
 * converts it into a single millisecond count.
 *
 * @return The current monotonic time, in milliseconds.
 */
long get_time_ms(void);

#endif /* !WORLD_H_ */
