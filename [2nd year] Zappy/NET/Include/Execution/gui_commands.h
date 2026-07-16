/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** gui_commands
*/

#ifndef GUI_COMMANDS_H_
    #define GUI_COMMANDS_H_

    #define NB_COMMANDS_GUI 9

    #include "world.h"

typedef struct {
    char *command;
    void (*handler)(world_t *world, gui_t *gui, char **args, int fd);
    size_t min_arg;
    size_t max_arg;
    size_t time_unit;
} command_gui_t;

const command_gui_t commands_gui[NB_COMMANDS_GUI] = {
    {"msz", &msz, 0, 0, 0},
    {"sgt", &sgt, 0, 0, 0},
    {"bct", &bct, 2, 2, 0},
    {"mct", &mct, 0, 0, 0},
    {"tna", &tna, 0, 0, 0},
    {"plv", &plv, 1, 1, 0},
    {"pin", &pin, 1, 1, 0},
    {"ppo", &ppo, 1, 1, 0},
    {"sst", &sst, 1, 1, 0},
};

#endif /* !GUI_COMMANDS_H_ */
