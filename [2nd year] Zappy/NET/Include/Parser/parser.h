/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** parser
*/

#ifndef PARSER_H
    #define PARSER_H

    #include <string.h>

    #include "args.h"
    #include "logger.h"
    #include "utils.h"
    #include "server.h"

typedef struct attributs_s attributs_t;

/**
 * @brief Parses a command line argument as an integer.
 *
 * Converts the given string argument into an unsigned long using
 * strtoul, ensures the whole string was a valid number, and checks
 * that the resulting value lies within the attribute's allowed
 * range via check_range. On any failure, the parsing_status field of
 * args is set to false and an error is logged.
 *
 * @param args Pointer to the args_t structure being filled, used to
 * report parsing errors.
 * @param arg String argument to parse as an integer.
 * @param attribut Pointer to the attributs_t structure describing
 * the argument's name and valid range.
 *
 * @return The parsed integer value on success, or 0 if the argument
 * is missing, not a valid integer, or out of range.
 */
size_t parse_integer_arg(args_t *args, char *arg, const attributs_t *attribut);

/**
 * @brief Parses the list of team names from the command line.
 *
 * Determines how many arguments after the current option index
 * belong to the team name list, allocates and fills a NULL-terminated
 * array of team name strings, then checks that all team names are
 * unique and that their number does not exceed the maximum allowed
 * (20). Any previously stored team names in args are freed first.
 *
 * @param args Pointer to the args_t structure being filled, used to
 * report parsing errors.
 * @param argv Array of command line argument strings.
 * @param argc Number of command line arguments.
 * @param optind Pointer to the current option index, updated to
 * point past the consumed team name arguments.
 *
 * @return A newly allocated, NULL-terminated array of team name
 * strings on success, or NULL if no team name was found, an
 * allocation failed, or the team names are invalid (empty, not
 * unique, or too many).
 */
char **parse_team_names(args_t *args, char *argv[], int argc, int *optind);

/**
 * @brief Parses the server's command line arguments.
 *
 * Initializes a default args_t structure, then iterates over the
 * command line options using getopt_long, dispatching each
 * recognized option to first_opt_case (and second_opt_case for
 * options not handled there). If parsing fails at any point or an
 * unexpected extra argument remains, the args_t structure is
 * destroyed, the usage message is displayed, and NULL is returned.
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line argument strings.
 *
 * @return A pointer to the newly allocated and filled args_t
 * structure on success, or NULL if initialization or parsing
 * failed.
 */
args_t *parse_args(int argc, char *argv[]);

/**
 * @brief Reads and splits the commands received from a connection.
 *
 * Receives data from the socket associated with the connection at
 * the given index in the poll_fds array. The received data is
 * appended to the connection's partial command buffer (to handle
 * commands split across several reads), then complete commands
 * (terminated by '\n') are extracted from that buffer via
 * flush_partial. If no data is received, the connection is
 * disconnected.
 *
 * @param server Pointer to the server_t structure holding the
 * network and its connections.
 * @param index Index of the connection in the poll_fds array.
 *
 * @return A newly allocated, NULL-terminated array of complete
 * command strings on success, or NULL if no data was received (the
 * connection was disconnected) or an allocation failed.
 */
char **get_split_commands(server_t *server, int index);

/**
 * @brief Parses a player number argument prefixed with '#'.
 *
 * Checks that the given string starts with a '#' character, then
 * converts the rest of the string into a long integer using strtol.
 * The conversion must consume the entire remaining string for it to
 * be considered valid.
 *
 * @param arg String argument to parse, expected to be in the form
 * "#<number>".
 *
 * @return The parsed player number on success, or -1 if the
 * argument is NULL, does not start with '#', or does not contain a
 * valid integer.
 */
int parse_player_number(char *arg);

#endif
