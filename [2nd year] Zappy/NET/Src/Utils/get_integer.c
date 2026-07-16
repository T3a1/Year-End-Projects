/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** get_interger
*/

#include "utils.h"

/**
 * @brief Converts a string into an integer.
 *
 * Parses the given string using strtol and checks that the whole
 * string was consumed as a valid number and that the resulting
 * value fits within the range of an int.
 *
 * @param str String to convert.
 * @param value Pointer to an int where the converted value is
 * stored on success.
 *
 * @return true if the string was successfully converted, false if
 * the string is not a valid integer or the value is out of range
 * for an int.
 */
bool get_integer(char *str, int *value)
{
    char *endptr = NULL;
    long int result = strtol(str, &endptr, 10);

    if (*endptr != '\0' || endptr == str)
        return false;
    if (result < INT_MIN || result > INT_MAX)
        return false;
    *value = (int)result;
    return true;
}
