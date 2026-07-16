/*
** EPITECH PROJECT, 2026
** SERVER
** File description:
** ai
*/

#ifndef AI_H_
    #define AI_H_

    #include "world.h"
    #include "resources.h"
    #include "map.h"

typedef enum {
    INITIAL,
    FORK
} egg_type_t;

typedef struct ai {
    size_t id;
    int id_owner;
    size_t level;
    team_t *team;
    direction_t direction;
    int inventory[RESOURCES_COUNT];
    tile_t *position;
    command_t *local_commands;
    egg_type_t egg_type;
    long last_check;
    tile_t *incant_tile;
    bool is_incanting;
    struct ai *next;
} ai_t;

/**
 * @brief Destroys an entire AI linked list.
 *
 * Iterates over the AI linked list starting from ai, freeing each
 * AI's local commands (and their arguments) as well as the AI node
 * itself, until the end of the list is reached.
 *
 * @param ai Pointer to the head of the AI linked list to destroy.
 *
 * @return None (void).
 */
void destroy_ais(ai_t *ai);

/**
 * @brief Destroys a single AI and removes it from the game state.
 *
 * Notifies every GUI client that the AI has died, then removes the
 * AI's pending commands from the global command queue, removes it
 * from its team, removes it from its current tile, and finally
 * removes it from the world's AI list (freeing its memory and its
 * local commands). The given pointer is set to NULL after
 * destruction. Does nothing if the pointed-to AI is already NULL.
 *
 * @param world Pointer to the world_t structure holding the
 * command queue and the AI list.
 * @param ai Pointer to the ai_t pointer to destroy.
 *
 * @return None (void).
 */
void destroy_ai(world_t *world, ai_t **ai);

/**
 * @brief Hatches a free egg into a level 1 AI for a given team.
 *
 * Looks for the team matching team_name, then looks for one of its
 * eggs (a player whose level is 0) via find_free_egg. If one is
 * found, it is promoted to level 1 and the team's free slot count is
 * decremented via init_ai.
 *
 * @param world Pointer to the world_t structure holding the team
 * list.
 * @param team_name Name of the team to hatch an egg for.
 *
 * @return A pointer to the newly hatched ai_t structure on success,
 * or NULL if no team matches team_name or no free egg is available.
 */
ai_t *bloom_egg(world_t *world, char *team_name);

/**
 * @brief Spawns the initial eggs for every team in the world.
 *
 * Iterates over every team in the world and spawns its initial
 * batch of eggs (as configured via eggs_per_team) on random tiles,
 * via spawn_team_eggs.
 *
 * @param world Pointer to the world_t structure holding the team
 * list and spawn configuration.
 *
 * @return true if every team's eggs were successfully spawned,
 * false if an allocation failed for any egg.
 */
bool spawn_eggs(world_t *world);

/**
 * @brief Spawns a new fork egg for a player who used the Fork command.
 *
 * Creates a new egg on the parent AI's current tile, marks it as a
 * FORK egg (as opposed to an INITIAL one), assigns it a new unique
 * id while remembering the parent's id as its owner, then registers
 * it in the world's AI list, on its tile, and in its team.
 *
 * @param world Pointer to the world_t structure holding the AI
 * list and id counter.
 * @param team Pointer to the team_t the new egg belongs to.
 * @param parent_ai Pointer to the ai_t structure of the player who
 * triggered the fork.
 *
 * @return A pointer to the newly created ai_t egg on success, or
 * NULL if the allocation failed.
 */
ai_t *spawn_fork_egg(world_t *world, team_t *team, ai_t *parent_ai);

/**
 * @brief Checks whether a team with the given name has a free slot.
 *
 * Looks for a team named team_name among the world's teams and
 * checks, via spot_available, that it still has at least one free
 * slot for a new player.
 *
 * @param world Pointer to the world_t structure holding the team
 * list.
 * @param team_name Name of the team to check.
 *
 * @return true if a team with that name exists and has a free slot,
 * false otherwise.
 */
bool is_available_team(world_t *world, char *team_name);

/**
 * @brief Checks whether any team has won the game.
 *
 * Iterates over every team in the world and checks, via team_wins,
 * whether it has at least 6 players at the maximum level (8). If
 * so, every connected GUI client is notified and the win is logged.
 *
 * @param world Pointer to the world_t structure holding the team
 * list and the GUI list.
 *
 * @return true if a team has won the game, false otherwise.
 */
bool win(world_t *world);

/**
 * @brief Removes and returns the player node matching a given AI.
 *
 * Looks for the player node whose ai field matches ai_to_remove in
 * the given linked list, unlinks it from the list (using pop_head
 * if it is the head, pop_inner otherwise), and returns it.
 *
 * @param head Pointer to the head of the player linked list.
 * @param ai_to_remove Pointer to the ai_t whose player node should
 * be removed from the list.
 *
 * @return A pointer to the removed player_t node (with its next
 * field reset to NULL), or NULL if no matching node was found.
 */
player_t *pop_ai_from_list(player_t **head, ai_t *ai_to_remove);

#endif
