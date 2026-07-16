/*
** EPITECH PROJECT, 2026
** Visual Studio Live Share (Workspace)
** File description:
** map
*/

#ifndef MAP_H_
    #define MAP_H_

    #include "resources.h"
    #include "utils.h"

typedef struct ai ai_t;

typedef enum {
    NORTH = 1,
    EAST = 2,
    SOUTH = 3,
    WEST = 4
} direction_t;

typedef struct player_s {
    ai_t *ai;
    struct player_s *next;
} player_t;

typedef struct tile_content {

    size_t resources[RESOURCES_COUNT];
    player_t *players;
} tile_content_t;

typedef struct tile {
    int x, y;
    struct tile *north;
    struct tile *south;
    struct tile *east;
    struct tile *west;
    tile_content_t *content;
} tile_t;

// Path finding structures

typedef struct {
    size_t x, y;
} pos2i_t;

typedef struct {
    int a, b;
} int_pair_t;

// Sound structure

typedef enum {
    ABS_NORTH_WEST,
    ABS_SOUTH_WEST,
    ABS_WEST,
    ABS_NORTH,
    ABS_SOUTH,
    ABS_EAST,
    ABS_SOUTH_EAST,
    ABS_NORTH_EAST
} absolute_direction_t;

/**
 * @brief Computes the shortest vector between two points on a torus.
 *
 * For each axis (x and y), computes the direct difference between
 * start and end as well as the "wrapped around" difference (taking
 * the world's toroidal topology into account), then keeps whichever
 * of the two has the smallest absolute value, via get_axis_vec.
 *
 * @param world Pointer to the world_t structure holding the map's
 * width and height.
 * @param start Pointer to the starting position.
 * @param end Pointer to the target position.
 *
 * @return An int_pair_t holding the shortest signed displacement
 * along the x axis (a) and the y axis (b) to go from start to end
 * on the torus.
 */
int_pair_t torus_shortest_vector(world_t *world,
    const pos2i_t *start, const pos2i_t *end);

/**
 * @brief Converts a displacement vector into a sound direction digit.
 *
 * Determines the absolute direction (one of the 8 compass points)
 * pointed to by the given vector via vector_to_origin, then
 * converts that absolute direction into a direction digit (1 to 8)
 * relative to the listener's facing direction, via
 * get_digit_from_direction. This matches the direction convention
 * used by the Zappy protocol for sound/broadcast messages.
 *
 * @param v Pointer to the displacement vector between the emitter
 * and the listener.
 * @param facing Direction the listener is currently facing.
 *
 * @return 0 if the vector is null (the listener is on the same
 * tile as the emitter), a value between 1 and 8 representing the
 * relative direction otherwise, or -1 if the facing direction is
 * invalid.
 */
int get_dir_from_vector(int_pair_t *v, direction_t facing);

#endif /* !MAP_H_ */
