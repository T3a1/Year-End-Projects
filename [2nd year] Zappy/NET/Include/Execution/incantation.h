/*
** EPITECH PROJECT, 2026
** Visual Studio Live Share (Workspace)
** File description:
** incantation
*/

#ifndef INCANTATION_H_
    #define INCANTATION_H_

typedef struct incantation_s {
    size_t nb_players;
    int resources[7];
} incantation_t;

static const incantation_t incantation_requirements[7] = {
    {1, {0, 1, 0, 0, 0, 0, 0}},
    {2, {0, 1, 1, 1, 0, 0, 0}},
    {2, {0, 2, 0, 1, 0, 2, 0}},
    {4, {0, 1, 1, 2, 0, 1, 0}},
    {4, {0, 1, 2, 1, 3, 0, 0}},
    {6, {0, 1, 2, 3, 0, 1, 0}},
    {6, {0, 2, 2, 2, 2, 2, 1}},
};

#endif /* !INCANTATION_H_ */
