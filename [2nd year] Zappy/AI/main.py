#!/usr/bin/env python3
#
# EPITECH PROJECT, 2026
# Zappy
# File description:
# AI main
#

import sys
import argparse
from AI.com_server import COM_SERVER
from AI.algo.algo import Algo


def parse_args():
    if "--help" in sys.argv or "-help" in sys.argv:
        print("USAGE: ./zappy_ai -p port -n name -h machine\n")
        print("option              description")
        print("-p port             port number")
        print("-n name             name of the team")
        print("-h machine          name of the machine; localhost by default")
        sys.exit(0)

    parser = argparse.ArgumentParser(add_help=False)
    parser.add_argument('-p', dest='port', type=int, required=True)
    parser.add_argument('-n', dest='name', type=str, required=True)
    parser.add_argument('-h', dest='machine', type=str, default="localhost")

    try:
        return parser.parse_args()
    except SystemExit:
        sys.exit(84)


def main():
    args = parse_args()

    connection = COM_SERVER(args.machine, args.port)
    game = Algo()

    if not connection.connexion():
        return 84

    response = connection.receive()
    print("Zappy AI")

    connection.send(f"{args.name}\n")

    client_num = connection.receive()
    if client_num:
        print(f"[INFO] Slots : {client_num.strip()}")

    map_size = connection.receive()
    if map_size:
        print(f"[INFO] Map : {map_size.strip()}")

    game.start(connection, "none", map_size, team_name=args.name)

    connection.deconnexion()


if __name__ == "__main__":
    main()