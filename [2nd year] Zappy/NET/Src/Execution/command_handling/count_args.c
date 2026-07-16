/*
** EPITECH PROJECT, 2026
** Visual Studio Live Share (Workspace)
** File description:
** count_args
*/

#include "command.h"

int count_args(char **args)
{
    int count = 0;

    for (; args[count] != NULL; count++);
    return count - 1;
}
