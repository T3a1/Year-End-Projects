/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** int_to_str
*/

#include "utils.h"

/**
 * @brief Converts an integer into a newly allocated string.
 *
 * Allocates a buffer large enough to hold any int value (including
 * the sign and the null terminator) and writes the decimal string
 * representation of value into it.
 *
 * @param value Integer value to convert.
 *
 * @return A newly allocated string containing the decimal
 * representation of value, or NULL if the allocation failed.
 */
char *int_to_str(int value)
{
    char *buffer = calloc(12, sizeof(char));

    if (buffer == NULL)
        return NULL;
    snprintf(buffer, sizeof(buffer), "%d", value);
    return buffer;
}
