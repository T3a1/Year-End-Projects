/*
** EPITECH PROJECT, 2026
** Visual Studio Live Share (Workspace)
** File description:
** command
*/

#ifndef COMMAND_H_
    #define COMMAND_H_

    #include <time.h>

    #include "server.h"
    #include "connection.h"

typedef struct command_s {
    char **arguments;
    ai_t *player;
    int player_fd;
    time_t remaining_time;
    void (*handler)
    (__attribute__((unused)) world_t *world, struct command_s *command);
    struct command_s *next;
} command_t;


/**
 * @brief Identifies and handles a connection of unknown type.
 *
 * Splits the given command into arguments. If the command is
 * "GRAPHIC", the connection is set up as a GUI client. Otherwise, if
 * the command matches an available team name, the connection is set
 * up as an AI client (an egg is hatched for that team).
 *
 * @param server Pointer to the server_t structure holding the
 * world and network state.
 * @param connection Pointer to the connection_t structure whose
 * type is still UNKNOWN.
 * @param command Raw command string received from the client,
 * expected to be either "GRAPHIC" or a team name.
 *
 * @return true if the connection was successfully identified and
 * set up as a GUI or AI client, false otherwise.
 */
bool unknown_handler(server_t *server, connection_t *connection, char *command);

/**
 * @brief Handles a command received from a GUI client.
 *
 * Splits the given command string into arguments and dispatches it
 * to the matching GUI command handler via execute_gui_command. Does
 * nothing if the command is empty, the client data is invalid, or
 * the arguments could not be split.
 *
 * @param world Pointer to the world_t structure.
 * @param data Client data union, expected to hold the gui client.
 * @param command Raw command string received from the GUI client.
 * @param fd File descriptor of the GUI client, used to send
 * responses or error codes.
 *
 * @return true if the command was successfully executed, false
 * otherwise.
 */
bool gui_handler(world_t *world, client_data_t cli,
    __attribute_maybe_unused__ char *command, int fd);

/**
 * @brief Handles a command received from an AI client.
 *
 * Splits the given command string into arguments and enqueues it as
 * a new command for the AI player via enqueue_ai_command. Does
 * nothing if the command is empty, the client data is invalid, or
 * the arguments could not be split. The arguments array is freed if
 * enqueueing failed (ownership is transferred to the command queue
 * on success).
 *
 * @param world Pointer to the world_t structure holding the command
 * queue.
 * @param data Client data union, expected to hold the ai client.
 * @param command Raw command string received from the AI client.
 * @param fd File descriptor of the AI client.
 *
 * @return true if the command was successfully enqueued, false
 * otherwise.
 */
bool ai_handler(world_t *world, client_data_t cli,
    __attribute_maybe_unused__ char *command, int fd);

/**
 * @brief Counts the number of arguments in a command, excluding its
 * name.
 *
 * Counts the total number of non-NULL elements in the
 * NULL-terminated array, then subtracts one to exclude the command
 * name itself (the first element).
 *
 * @param args NULL-terminated array of command arguments, whose
 * first element is the command name.
 *
 * @return The number of arguments following the command name.
 */
int count_args(char **args);

/**
 * @brief Creates a new command_t structure.
 *
 * Allocates a new command_t structure and fills it with the given
 * arguments, player, remaining time and player file descriptor.
 * Refuses to create a command without a player unless the command
 * is "REFILL", and refuses to create a command for a player who is
 * currently incanting.
 *
 * @param command NULL-terminated array of command arguments, whose
 * first element is the command name.
 * @param player Pointer to the ai_t structure owning the command,
 * or NULL for the special "REFILL" command.
 * @param remaining_time Time, in milliseconds, before the command
 * should be executed.
 * @param player_fd File descriptor of the player issuing the
 * command.
 *
 * @return A pointer to the newly allocated command_t structure on
 * success, or NULL if the allocation failed, the player is missing
 * for a non-"REFILL" command, or the player is currently incanting.
 */
command_t *create_command(char **command, ai_t *player,
    time_t remaining_time, int player_fd);

/**
 * @brief Inserts a new command into the global command queue.
 *
 * If the command is an "Incantation", performs pre-incantation
 * checks via pre_incantation_check; if these checks fail, the
 * command is discarded and freed. Otherwise, the command is
 * inserted into the global queue (sorted by remaining time) via
 * insert_global_command, and the world's timing fields are updated.
 *
 * @param world Pointer to the world_t structure holding the global
 * command queue and timing information.
 * @param new_command Pointer to the command_t structure to insert.
 *
 * @return None (void).
 */
void add_global_command(world_t *world, command_t *new_command);

/**
 * @brief Adds a new command for a player to the appropriate queue.
 *
 * If the player already has a pending command in the global queue,
 * the new command is appended to that player's local command queue
 * (to be promoted later). Otherwise, the command is added directly
 * to the global command queue via add_global_command.
 *
 * @param world Pointer to the world_t structure holding the global
 * command queue.
 * @param new_command Pointer to the command_t structure to add.
 *
 * @return None (void).
 */
void add_command(world_t *world, command_t *new_command);

/**
 * @brief Executes a single command from the command queue.
 *
 * If the command is the special "REFILL" command, spawns resources
 * on the map via execute_refill. Otherwise, validates the command
 * (handler and player presence) via validate_command, then calls
 * its handler function.
 *
 * @param server Pointer to the server_t structure holding the
 * world state.
 * @param command Pointer to the command_t structure to execute.
 *
 * @return true if the command was the "REFILL" command and was
 * executed successfully, false in every other case (including
 * after a regular command's handler has run, or if validation
 * failed).
 */
bool execute_command(server_t *server, command_t *command);

/**
 * @brief Refreshes the global command queue for the elapsed time.
 *
 * Computes the time elapsed since the last refresh, then walks the
 * global command queue: commands whose remaining time is still
 * above the elapsed time simply have it subtracted, while commands
 * that have reached zero are executed and removed via
 * process_command (which may also queue a player's next local
 * command for promotion). Once the queue has been walked, every
 * promoted command is reinserted into the global queue, and the
 * world's timing fields are updated.
 *
 * @param server Pointer to the server_t structure holding the
 * world and its command queue.
 *
 * @return None (void).
 */
void refresh_commands(server_t *server);

/**
 * @brief Finds the handler function for a given AI command.
 *
 * Looks up the commands_ai table for an entry whose command name
 * matches the first element of the given argument array.
 *
 * @param command Array of command arguments, whose first element
 * is the command name to look up.
 *
 * @return A pointer to the matching command handler function, or
 * NULL if no command with that name was found.
 */
void (*get_handler(char **command))(world_t *world, command_t *command);

/**
 * @brief Executes every command received from a client.
 *
 * Retrieves the connection matching the given client_id, then
 * dispatches each command in the given array to the appropriate
 * handler via send_to_handler, based on the connection's type (GUI,
 * AI, or UNKNOWN). If a command fails for an AI client, an error
 * code is sent back. If the firewall is enabled, a client whose
 * command failed is banned and disconnected via handle_firewall.
 *
 * @param server Pointer to the server_t structure holding the
 * network and its connections.
 * @param commands NULL-terminated array of command strings to
 * execute.
 * @param client_id Index of the client's connection in the
 * poll_fds array.
 *
 * @return None (void).
 */
void execute_commands
(server_t *server, char **commands, unsigned int client_id);

/**
 * @brief Sends the full initial state to a newly connected GUI client.
 *
 * Successively calls the map size, time unit, map content, team
 * names and existing AI commands so that the GUI client receives
 * the complete current state of the server upon connection.
 *
 * @param world Pointer to the world_t structure holding the current
 * game state.
 * @param gui Pointer to the gui_t structure representing the GUI
 * client.
 * @param args Optional array of command arguments forwarded to each
 * sub-command.
 * @param fd File descriptor of the GUI client to send the state to.
 *
 * @return None (void).
 */
void graphic(world_t *world, gui_t *gui, char **args, int fd);

// Gui commands
void bct(__attribute__((unused)) world_t *world, gui_t *gui,
    __attribute__((unused)) char **args, int fd);
void mct(__attribute__((unused)) world_t *world, gui_t *gui,
    __attribute__((unused)) char **args, int fd);
void msz(__attribute__((unused)) world_t *world, gui_t *gui,
    __attribute__((unused)) char **args, int fd);
void pin(__attribute__((unused)) world_t *world, gui_t *gui,
    __attribute__((unused)) char **args, int fd);
void plv(__attribute__((unused)) world_t *world, gui_t *gui,
    __attribute__((unused)) char **args, int fd);
void ppo(__attribute__((unused)) world_t *world, gui_t *gui,
    __attribute__((unused)) char **args, int fd);
void sgt(__attribute__((unused)) world_t *world, gui_t *gui,
    __attribute__((unused)) char **args, int fd);
void sst(__attribute__((unused)) world_t *world, gui_t *gui,
    __attribute__((unused)) char **args, int fd);
void tna(__attribute__((unused)) world_t *world, gui_t *gui,
    __attribute__((unused)) char **args, int fd);
void enw(__attribute__((unused)) world_t *world, gui_t *gui,
    __attribute__((unused)) char **args, int fd);
void graphic(__attribute__((unused)) world_t *world, gui_t *gui,
    __attribute__((unused)) char **args, int fd);

void enw(__attribute__((unused)) world_t *world, gui_t *gui,
    __attribute__((unused)) char **args, int fd);

void ebo(__attribute__((unused)) world_t *world, ai_t *ai, int fd,
    __attribute__((unused)) char *arg);
void edi(__attribute__((unused)) world_t *world, ai_t *ai, int fd,
    __attribute__((unused)) char *arg);
void pbc(__attribute__((unused)) world_t *world, ai_t *ai, int fd,
    __attribute__((unused)) char *arg);
void pdi(__attribute__((unused)) world_t *world, ai_t *ai, int fd,
    __attribute__((unused)) char *arg);
void pdr(__attribute__((unused)) world_t *world, ai_t *ai, int fd,
    __attribute__((unused)) char *arg);
void pex(__attribute__((unused)) world_t *world, ai_t *ai, int fd,
    __attribute__((unused)) char *arg);
void pfk(__attribute__((unused)) world_t *world, ai_t *ai, int fd,
    __attribute__((unused)) char *arg);
void pgt(__attribute__((unused)) world_t *world, ai_t *ai, int fd,
    __attribute__((unused)) char *arg);
void pic(__attribute__((unused)) world_t *world, ai_t *ai, int fd,
    __attribute__((unused)) char *arg);
void pie(__attribute__((unused)) world_t *world, ai_t *ai, int fd,
    __attribute__((unused)) char *arg);
void pnw(__attribute__((unused)) world_t *world, ai_t *ai, int fd,
    __attribute__((unused)) char *arg);

// AI commands
void look(__attribute__((unused)) world_t *world, command_t *command);
void connect_nbr(__attribute__((unused)) world_t *world, command_t *command);
void forward(__attribute__((unused)) world_t *world, command_t *command);
void right(__attribute__((unused)) world_t *world, command_t *command);
void left(__attribute__((unused)) world_t *world, command_t *command);
void inventory(__attribute__((unused)) world_t *world, command_t *command);
void take(__attribute__((unused)) world_t *world, command_t *command);
void set(__attribute__((unused)) world_t *world, command_t *command);
void zfork(__attribute__((unused)) world_t *world, command_t *command);
void incantation(__attribute__((unused)) world_t *world, command_t *command);
void broadcast(__attribute__((unused)) world_t *world, command_t *command);
void eject(__attribute__((unused)) world_t *world, command_t *command);

/**
 * @brief Returns the tile located to the left of a given direction.
 *
 * Returns the neighboring tile of t that lies to the left relative
 * to the direction pointed to by dir.
 *
 * @param t Pointer to the reference tile.
 * @param dir Direction currently faced.
 *
 * @return A pointer to the tile_t structure located to the left, or
 * NULL if dir is invalid.
 */
tile_t *get_left(tile_t *t, direction_t dir);

/**
 * @brief Returns the tile located to the right of a given direction.
 *
 * Returns the neighboring tile of t that lies to the right relative
 * to the direction pointed to by dir.
 *
 * @param t Pointer to the reference tile.
 * @param dir Direction currently faced.
 *
 * @return A pointer to the tile_t structure located to the right,
 * or NULL if dir is invalid.
 */
tile_t *get_right(tile_t *t, direction_t dir);

/**
 * @brief Returns the tile located in front of a given direction.
 *
 * Returns the neighboring tile of t that lies in the direction
 * pointed to by dir.
 *
 * @param t Pointer to the reference tile.
 * @param dir Direction currently faced.
 *
 * @return A pointer to the tile_t structure located in front, or
 * NULL if dir is invalid.
 */
tile_t *get_front(tile_t *t, direction_t dir);

/**
 * @brief Validates the number of arguments for the Broadcast command.
 *
 * Counts the arguments following the command name and checks that
 * exactly one argument (the message to broadcast) is present.
 *
 * @param command NULL-terminated array of command arguments, whose
 * first element is the command name.
 *
 * @return true if exactly one argument is present, false otherwise.
 */
bool check_broadcast_command(char **command);

/**
 * @brief Converts a resource name string into a resource_t value.
 *
 * Compares the given string against the known resource names
 * ("food", "linemate", "deraumere", "sibur", "mendiane", "phiras",
 * "thystame") and returns the matching resource_t value.
 *
 * @param str Name of the resource to convert.
 *
 * @return The matching resource_t value, or -1 if str does not
 * match any known resource name.
 */
resource_t str_to_resource(char *str);


// Look struct
typedef struct {
    char *data;
    size_t cap;
} buf_t;

/**
 * @brief Checks whether an incantation can start or continue on a tile.
 *
 * Verifies that the player's level allows an incantation (between 1
 * and 7). If the player is already incanting, ensures they are
 * still on the tile where the incantation started. Otherwise,
 * checks that the tile holds enough resources and enough players
 * for the player's level via enough_resources and enough_players,
 * and, if the incantation is just starting, marks the tile's
 * incantation state via set_incantation_state.
 *
 * @param world Pointer to the world_t structure holding the map.
 * @param command Pointer to the command_t structure representing
 * the incantation command, whose player is attempting the
 * incantation.
 *
 * @return true if the incantation can start or continue, false
 * otherwise.
 */
bool verify_incantation(world_t *world, command_t *command);

#endif
