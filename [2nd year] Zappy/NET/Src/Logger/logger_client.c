/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** logger_client
*/

#include "logger.h"

static char *setup_log(const char *message, va_list args)
{
    char *buffer = calloc(1, MAX_LOG_SIZE);

    if (!buffer)
        return NULL;
    vsnprintf(buffer, MAX_LOG_SIZE, message, args);
    return buffer;
}

void logger_client(int client_fd, char *message, ...)
{
    int res = 0;
    va_list args;
    char *log_message = NULL;

    va_start(args, message);
    va_end(args);
    log_message = setup_log(message, args);
    if (!log_message)
        return;
    res = dprintf(client_fd, "%s\n", log_message);
    if (res < 0)
        logger_server("Failed to send message '%s' to client", WARNING,
            log_message);
    free(log_message);
}
