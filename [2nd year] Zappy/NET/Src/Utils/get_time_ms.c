/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** get_time_ms
*/

#include <time.h>
#include "utils.h"

long get_time_ms(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000L + ts.tv_nsec / 1000000L);
}
