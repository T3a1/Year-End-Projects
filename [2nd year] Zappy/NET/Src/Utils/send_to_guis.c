/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** send_to_guis
*/

#include "gui.h"
#include "logger.h"

static char *setup_log(const char *message, va_list args)
{
    char *buffer = malloc(MAX_LOG_SIZE);

    if (!buffer)
        return NULL;
    vsnprintf(buffer, MAX_LOG_SIZE, message, args);
    return buffer;
}

void send_to_guis(gui_t *current_guis, char *message, ...)
{
    va_list args;
    char *msg = NULL;

    va_start(args, message);
    va_end(args);
    msg = setup_log(message, args);
    while (current_guis) {
        logger_client(current_guis->fd, "%s", msg);
        current_guis = current_guis->next;
    }
    free(msg);
}
