/*
** EPITECH PROJECT, 2026
** Visual Studio Live Share (Workspace)
** File description:
** ai_commands
*/

#ifndef AI_COMMANDS_H_
    #define AI_COMMANDS_H_
    #define NB_COMMANDS_AI 12

    #include "command.h"

typedef struct {
    char *command;
    void (*handler)(world_t *world, command_t *command);
    size_t min_arg;
    size_t max_arg;
    size_t time_unit;
} command_ai_t;

const command_ai_t commands_ai[NB_COMMANDS_AI] = {
    {"Look", &look, 0, 0, 7},
    {"Connect_nbr", &connect_nbr, 0, 0, 0},
    {"Forward", &forward, 0, 0, 7},
    {"Left", &left, 0, 0, 7},
    {"Right", &right, 0, 0, 7},
    {"Inventory", &inventory, 0, 0, 1},
    {"Take", &take, 1, 1, 7},
    {"Set", &set, 1, 1, 7},
    {"Fork", &zfork, 0, 0, 42},
    {"Incantation", &incantation, 0, 0, 300},
    {"Broadcast", &broadcast, 1, 1, 7},
    {"Eject", &eject, 0, 0, 7}
};

#endif /* !AI_COMMANDS_H_ */
