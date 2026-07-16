/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** split_arguments
*/

#include "utils.h"

/**
 * @brief Splits a string into an array of tokens.
 *
 * Tokenizes the given string using the provided set of delimiters
 * (via strtok), dynamically growing an array of duplicated tokens as
 * they are found.
 *
 * @param str String to split. Note that strtok modifies this string
 * in place.
 * @param delimiters String containing the set of delimiter
 * characters.
 *
 * @return A newly allocated, NULL-terminated array of token strings,
 * or NULL if no token was found.
 */
char **split_arguments(char *str, char *delimiters)
{
    char **array = NULL;
    char *token = NULL;
    int count = 0;

    token = strtok(str, delimiters);
    while (token != NULL) {
        array = realloc(array, sizeof(char *) * (count + 2));
        array[count] = strdup(token);
        count++;
        token = strtok(NULL, delimiters);
    }
    if (array)
        array[count] = NULL;
    return array;
}
