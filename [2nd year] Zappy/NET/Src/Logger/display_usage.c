/*
** EPITECH PROJECT, 2026
** SERVER
** File description:
** display_usage
*/

#include "logger.h"

void display_usage(void)
{
    printf("USAGE: ./zappy_server -p port -x width -y height ");
    printf("-n name1 name2 ... -c clientsNb -f freq\n [-v | --verbose] ");
    printf("[-h | --help] [-w | --firewall]\n");
}
