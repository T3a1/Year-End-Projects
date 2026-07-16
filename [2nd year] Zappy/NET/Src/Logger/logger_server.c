/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** logger_server
*/

#include "logger.h"

static char *setup_log(const char *message, va_list args)
{
    char *buffer = malloc(MAX_LOG_SIZE);

    if (!buffer)
        return NULL;
    vsnprintf(buffer, MAX_LOG_SIZE, message, args);
    return buffer;
}

static char *get_current_date(void)
{
    time_t current_time = time(NULL);
    char *string_time = ctime(&current_time);

    string_time[strcspn(string_time, "\n")] = '\0';
    return string_time;
}

static void display_log(char *current_time, char *message, logger_type_t type)
{
    const logger_t logger[] = {
        { .type = CRITICAL, .type_str = "CRITICAL", .color = RED },
        { .type = WARNING, .type_str = "WARNING", .color = YELLOW },
        { .type = MAJOR, .type_str = "MAJOR", .color = ORANGE },
        { .type = MINOR, .type_str = "MINOR", .color = PURPLE },
        { .type = OK, .type_str = "OK", .color = GREEN },
        { .type = INFO, .type_str = "INFO", .color = BLUE }
    };

    fprintf(stdout, "%s[%s] %s - %s%s\n", logger[type].color, current_time,
        logger[type].type_str, message, RESET);
}

void logger_server(char *message, logger_type_t type, ...)
{
    char *current_time = get_current_date();
    char *log_message = NULL;
    va_list args;

    va_start(args, type);
    va_end(args);
    log_message = setup_log(message, args);
    display_log(current_time, log_message, type);
    free(log_message);
}
