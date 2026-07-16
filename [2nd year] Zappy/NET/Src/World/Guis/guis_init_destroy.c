/*
** EPITECH PROJECT, 2026
** Visual Studio Live Share (Workspace)
** File description:
** init_destroy
*/

#include "gui.h"
#include "logger.h"
#include "connection.h"

void destroy_gui(gui_t *gui)
{
    gui_t *current = gui;
    gui_t *next = NULL;

    if (gui == NULL)
        return;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

static void second_remove_gui(gui_t *previous, gui_t *current, gui_t **head)
{
    if (previous == NULL) {
        *head = current->next;
    } else {
        previous->next = current->next;
    }
    free(current);
    return;
}

void remove_gui(gui_t **head, gui_t *gui_to_remove)
{
    gui_t *current = *head;
    gui_t *previous = NULL;

    while (current != NULL) {
        if (current->fd == gui_to_remove->fd) {
            return second_remove_gui(previous, gui_to_remove, head);
        }
        previous = current;
        current = current->next;
    }
}

void new_gui(world_t *world, connection_t *connection)
{
    gui_t *new_gui = calloc(1, sizeof(gui_t));

    if (new_gui == NULL) {
        logger_server("Failed to allocate memory for new GUI", MAJOR);
        return;
    }
    new_gui->next = world->gui;
    world->gui = new_gui;
    new_gui->fd = connection->fd;
    connection->cli.gui = new_gui;
}
