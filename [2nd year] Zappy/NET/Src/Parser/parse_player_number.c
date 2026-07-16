/*
** EPITECH PROJECT, 2026
** SERVER
** File description:
** parse_player_number
*/

#include "parser.h"

int parse_player_number(char *arg)
{
    char *end = NULL;
    long value = 0;

    if (!arg || arg[0] != '#')
        return -1;
    value = strtol(arg + 1, &end, 10);
    if (end == arg + 1 || *end != '\0')
        return -1;
    return (int)value;
}
