/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** sst
*/

#include "command.h"

static void change_local_commands(ai_t *ais, int old_freq, int new_freq)
{
    command_t *current = NULL;

    while (ais != NULL) {
        current = ais->local_commands;
        while (current != NULL) {
            current->remaining_time =
                (current->remaining_time * old_freq) / new_freq;
            current = current->next;
        }
        ais = ais->next;
    }
}

static void change_global_commands
(command_t *commands, int old_freq, int new_freq)
{
    command_t *current = commands;

    while (current != NULL) {
        current->remaining_time =
            (current->remaining_time * old_freq) / new_freq;
        current = current->next;
    }
}

void sst(world_t *world, __attribute__((unused)) gui_t *gui,
    __attribute__((unused)) char **args, int fd)
{
    int freq = 0;

    if (!get_integer(args[1], &freq)) {
        logger_client(fd, "sbp");
        return;
    }
    if (freq < 1 || freq > 150) {
        logger_client(fd, "sbp");
        return;
    }
    change_local_commands(world->ais, world->args->freq, freq);
    change_global_commands(world->command_queue, world->args->freq, freq);
    world->args->freq = freq;
    logger_client(fd, "sst %d", freq);
}
