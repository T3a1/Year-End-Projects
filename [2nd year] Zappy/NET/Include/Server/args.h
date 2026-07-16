/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** args
*/

#ifndef ARGS_H_
    #define ARGS_H_

    #include <stdint.h>
    #include <stddef.h>
    #include <stdbool.h>
    #include <stdlib.h>
    #include <string.h>

    #include "utils.h"

typedef struct {
    uint16_t port;
    size_t width;
    size_t height;
    size_t eggs_per_team;
    size_t freq;
    char **team_names;
    bool firewall;
    bool verbose;
    bool parsing_status;
} args_t;

/**
 * @brief Initializes an args_t structure with default values.
 *
 * Dynamically allocates the args_t structure as well as the team_names
 * array, then initializes all fields with the Zappy server's default
 * values: port, map width and height, eggs per team, frequency, the
 * first team's name, firewall state, parsing status and verbosity.
 *
 * @return A pointer to the newly allocated and initialized args_t
 * structure on success, or NULL if allocation of the structure or of
 * the team_names array failed.
 */
args_t *init_args(void);

/**
 * @brief Frees the memory allocated for an args_t structure.
 *
 * Frees the team_names array via free_array, then frees the args_t
 * structure itself. The function does nothing if the given pointer
 * is NULL.
 *
 * @param args Pointer to the args_t structure to destroy.
 *
 * @return None (void).
 */
void destroy_args(args_t *args);

#endif
