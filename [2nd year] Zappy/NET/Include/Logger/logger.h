/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** logger
*/

#ifndef LOGGER_H_
    #define LOGGER_H_

    #include <stdio.h>
    #include <stdarg.h>
    #include <time.h>

    #include "args.h"

    #define MAX_LOG_SIZE 1024

    #define HEADER_SIZE 64
    #define PLAYER_ENTRY_SIZE 24

    #define COLOR_SIZE 12

    #define RED "\033[31m"
    #define ORANGE "\033[38;5;208m"
    #define YELLOW "\033[33m"
    #define PURPLE "\033[38;5;93m"
    #define GREEN "\033[32m"
    #define BLUE "\033[34m"
    #define BOLD "\033[1;37m"
    #define RESET "\033[0m"

typedef enum {
    CRITICAL,
    WARNING,
    MAJOR,
    MINOR,
    OK,
    INFO
} logger_type_t;

typedef struct server_s server_t;

typedef struct {
    logger_type_t type;
    char *type_str;
    char color[COLOR_SIZE];
} logger_t;

/**
 * @brief Displays the server's usage message.
 *
 * Prints a short summary of the command line options expected by
 * the Zappy server to standard output.
 *
 * @return None (void).
 */
void display_usage(void);

/**
 * @brief Displays the help message if requested on the command line.
 *
 * Looks for a "-h" or "--help" flag among the given command line
 * arguments. If found, prints the usage message followed by a
 * detailed description of every available option.
 *
 * @param argv NULL-terminated array of command line argument
 * strings to search for the help flag.
 *
 * @return true if the help flag was found and the help message was
 * displayed, false otherwise.
 */
bool display_help(char *argv[]);

/**
 * @brief Displays the server's current configuration.
 *
 * Prints a formatted summary of the server's arguments (port,
 * dimensions, eggs per team, frequency, team names, firewall and
 * verbose flags) as well as the maximum number of connections, to
 * standard output.
 *
 * @param server Pointer to the server_t structure whose
 * configuration should be displayed.
 *
 * @return None (void).
 */
void display_config(server_t *server);

/**
 * @brief Logs a formatted message on the server side.
 *
 * Builds the current timestamp, formats the given message (with its
 * variadic arguments) into a buffer, then displays it on standard
 * output prefixed with the timestamp and colored according to the
 * given log type.
 *
 * @param message Format string of the message to log, following
 * printf-style formatting.
 * @param type Severity/category of the log message (CRITICAL,
 * WARNING, MAJOR, MINOR, OK or INFO).
 * @param ... Variadic arguments matching the message format string.
 *
 * @return None (void).
 */
void logger_server(char *message, logger_type_t type, ...);

/**
 * @brief Sends a formatted log message to a client.
 *
 * Formats the given message (with its variadic arguments) into a
 * buffer, then sends it to the client through its file descriptor
 * via dprintf. If sending fails, a warning is logged on the server
 * side.
 *
 * @param client_fd File descriptor of the client to send the
 * message to.
 * @param message Format string of the message to send, following
 * printf-style formatting.
 * @param ... Variadic arguments matching the message format string.
 *
 * @return None (void).
 */
void logger_client(int client_fd, char *message, ...);

#endif
