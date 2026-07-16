/*
** EPITECH PROJECT, 2026
** Visual Studio Live Share (Workspace)
** File description:
** team
*/

#ifndef TEAM_H_
    #define TEAM_H_

    #include "ai.h"
    #include "map.h"

typedef struct team {
    char *name;
    size_t free_slots;
    player_t *players;
    struct team *next;
} team_t;

/**
 * @brief Initializes the world's team list from the parsed arguments.
 *
 * Creates a new team_t structure for each team name found in the
 * server's arguments (name and initial free slot count), and links
 * them all into a single linked list assigned to world->teams. If
 * any team allocation fails, every previously created team is freed
 * and the world's team list is left unset.
 *
 * @param world Pointer to the world_t structure to fill, whose args
 * field holds the list of team names.
 *
 * @return true if every team was successfully created, false if an
 * allocation failed for any team.
 */
bool init_teams(world_t *world);

/**
 * @brief Frees an entire team linked list.
 *
 * Iterates over the linked list starting from teams. For each team,
 * frees its name and every player node in its player list, then
 * frees the team node itself, until the end of the list is reached.
 *
 * @param teams Pointer to the head of the team linked list to
 * destroy.
 *
 * @return None (void).
 */
void free_teams(team_t *teams);

#endif /* !TEAM_H_ */
