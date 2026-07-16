/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** look
*/

#include "command.h"
#include "world.h"
#include "map.h"
#include "logger.h"

static void add_inventory_to_output(char *buffer, ai_t *player)
{
    sprintf(buffer, "[food %d, linemate %d, deraumere %d, sibur %d, "
        "mendiane %d, phiras %d, thystame %d]",
        player->inventory[FOOD], player->inventory[LINEMATE],
        player->inventory[DERAUMERE], player->inventory[SIBUR],
        player->inventory[MENDIANE], player->inventory[PHIRAS],
        player->inventory[THYSTAME]);
}

void inventory(__attribute__((unused)) world_t *world, command_t *command)
{
    char output_buffer[BUFFER_SIZE] = {0};

    add_inventory_to_output(output_buffer, command->player);
    logger_client(command->player_fd, output_buffer);
}
