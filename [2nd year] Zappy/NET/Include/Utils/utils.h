/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** utils
*/

#ifndef UTILS_H_
    #define UTILS_H_

    #include <stddef.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <limits.h>
    #include <stdio.h>
    #include <string.h>
    #include <stdarg.h>

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
void free_array(char **array);

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
char *int_to_str(int value);

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
bool get_integer(char *str, int *value);

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
char **split_arguments(char *str, char *delimiters);
long get_time_ms(void);


#endif
