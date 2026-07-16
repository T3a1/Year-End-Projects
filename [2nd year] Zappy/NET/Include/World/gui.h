/*
** EPITECH PROJECT, 2026
** SERVER
** File description:
** gui
*/

#ifndef GUI_H_
    #define GUI_H_

    #include "world.h"

    #define MAX_LOG_SIZE 1024


typedef union client_data_u client_data_t;

typedef struct gui {
    int fd;
    struct gui *next;
} gui_t;

/**
 * @brief Creates a new GUI node and links it to a connection.
 *
 * Allocates a new gui_t structure, inserts it at the head of the
 * world's GUI linked list, sets its file descriptor from the given
 * connection, and links the connection's client data back to the
 * new GUI node.
 *
 * @param world Pointer to the world_t structure holding the GUI
 * list.
 * @param connection Pointer to the connection_t structure
 * representing the new GUI client.
 *
 * @return None (void).
 */
void new_gui(world_t *world, connection_t *connection);

/**
 * @brief Frees an entire GUI linked list.
 *
 * Iterates over the linked list starting from gui, freeing each
 * node one by one until the end of the list is reached. Does
 * nothing if gui is NULL.
 *
 * @param gui Pointer to the head of the GUI linked list to destroy.
 *
 * @return None (void).
 */
void destroy_gui(gui_t *gui);

/**
 * @brief Removes and frees a GUI node from the GUI linked list.
 *
 * Looks for the node whose fd field matches gui_to_remove's fd in
 * the linked list, then unlinks and frees it via second_remove_gui.
 * Does nothing if no matching node is found.
 *
 * @param head Pointer to the head of the GUI linked list.
 * @param gui_to_remove Pointer to the gui_t structure whose file
 * descriptor identifies the node to remove.
 *
 * @return None (void).
 */
void remove_gui(gui_t **head, gui_t *gui_to_remove);

/**
 * @brief Sends a formatted message to every connected GUI client.
 *
 * Formats the given message (with its variadic arguments) into a
 * buffer via setup_log, then sends it to every GUI client in the
 * given linked list.
 *
 * @param current_guis Pointer to the head of the GUI linked list to
 * send the message to.
 * @param message Format string of the message to send, following
 * printf-style formatting.
 * @param ... Variadic arguments matching the message format string.
 *
 * @return None (void).
 */
void send_to_guis(gui_t *current_guis, char *message, ...);

/**
 * @brief Notifies every GUI client that an incantation has started.
 *
 * Builds the incantation notification message (tile coordinates,
 * level, and the ids of every player of that level on the tile) via
 * build_incantation_message, then sends it to every GUI client in
 * the given linked list.
 *
 * @param guis Pointer to the head of the GUI linked list to notify.
 * @param tile Pointer to the tile_t where the incantation is taking
 * place.
 * @param level Level of the incantation.
 *
 * @return None (void).
 */
void logger_incantation(gui_t *guis, tile_t *tile, size_t level);

#endif
