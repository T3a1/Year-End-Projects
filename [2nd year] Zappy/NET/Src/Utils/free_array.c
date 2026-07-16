/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** free_array
*/

#include "utils.h"

/**
 * @brief Frees a NULL-terminated array of strings.
 *
 * Iterates over the array, freeing each string until the
 * terminating NULL pointer is reached, then frees the array itself.
 * Does nothing if the given pointer is NULL.
 *
 * @param array NULL-terminated array of strings to free.
 *
 * @return None (void).
 */
void free_array(char **array)
{
    if (!array)
        return;
    for (size_t i = 0; array[i]; i++)
        free(array[i]);
    free(array);
}
