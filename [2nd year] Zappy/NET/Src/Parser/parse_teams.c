/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** parse_teams
*/

#include "parser.h"

static char **free_team_names(char **team_names, int count)
{
    for (int i = count - 1; i >= 0; i--)
        free(team_names[i]);
    free(team_names);
    return NULL;
}

static bool validate_team_arg(char **teams, char *argv[], int idx, int i)
{
    if (!argv[idx] || argv[idx][0] == '\0') {
        logger_server("Error: Team name cannot be empty", CRITICAL);
        free_team_names(teams, i);
        return false;
    }
    return true;
}

static bool fill_team_name(char **teams, char *argv[], int start, int count)
{
    for (int i = 0; i < count; i++) {
        if (!validate_team_arg(teams, argv, start + i, i))
            return false;
        teams[i] = strdup(argv[start + i]);
        if (!teams[i]) {
            free_team_names(teams, i);
            return false;
        }
    }
    teams[count] = NULL;
    return true;
}

static int get_team_count(int argc, char *argv[], int *optind, int start)
{
    while (*optind < argc && argv[*optind] && argv[*optind][0] != '-')
        (*optind)++;
    return *optind - start;
}

static bool is_valid_team(char *teams[], size_t index)
{
    for (size_t i = 0; teams[i] != NULL; i++) {
        if (i == index)
            continue;
        if (!strcmp(teams[i], teams[index])) {
            logger_server("Error: Team names must be different", CRITICAL);
            return false;
        }
    }
    return true;
}

static bool check_unique_teams(args_t *args, char **team_names)
{
    for (size_t i = 0; team_names[i] != NULL; i++) {
        if (!is_valid_team(team_names, i)) {
            free_array(team_names);
            args->parsing_status = false;
            return false;
        }
        if (i > 19) {
            logger_server("Error: Maximum number of teams exceeded "
                "(must be between 1 and 20)", CRITICAL);
            free_array(team_names);
            args->parsing_status = false;
            return false;
        }
    }
    return true;
}

static char **alloc_and_fill(args_t *args, char *argv[], int start, int count)
{
    char **team_names = calloc(1, sizeof(char *) * (count + 1));

    if (!team_names) {
        args->parsing_status = false;
        return NULL;
    }
    args->parsing_status = fill_team_name(team_names, argv, start, count);
    if (!args->parsing_status)
        return NULL;
    return team_names;
}

char **parse_team_names(args_t *args, char *argv[], int argc, int *optind)
{
    int start = *optind - 1;
    int team_count = get_team_count(argc, argv, optind, start);
    char **team_names = NULL;

    if (args->team_names) {
        free_array(args->team_names);
        args->team_names = NULL;
    }
    if (team_count <= 0) {
        args->parsing_status = false;
        return NULL;
    }
    team_names = alloc_and_fill(args, argv, start, team_count);
    if (!team_names)
        return NULL;
    if (!check_unique_teams(args, team_names))
        return NULL;
    return team_names;
}
