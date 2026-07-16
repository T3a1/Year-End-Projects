#!/usr/bin/env python3
#
# EPITECH PROJECT, 2026
# Zappy
# File description:
# AI main (bonus : auto-fork des IA enfants selon les slots disponibles)
#

import sys
import argparse
import subprocess
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


def connect(args):
    connection = COM_SERVER(args.machine, args.port)
    game = Algo()

    if not connection.connexion():
        return 84

    response = connection.receive()
    print("Zappy AI")

    connection.send(f"{args.name}\n")

    client_num_str = connection.receive()
    if client_num_str is None or client_num_str.strip() == "ko":
        print("[ERREUR] Connexion refusee.")
        connection.deconnexion()
        return

    lines = client_num_str.strip().splitlines()
    try:
        client_num = int(lines[0].strip())
        print(f"[INFO] Slots restants : {client_num}")
        # Spawn en cascade : chaque IA spawne la suivante si des slots
        # sont deja disponibles au moment de la connexion.
        # Les slots ouverts par Fork seront geres par response_handler.
        if client_num > 0:
            print(f"[INFO] {client_num} slot(s) deja dispo -> spawn enfant")
            subprocess.Popen(
                [sys.executable, "-m", "AI.Bonus"] + sys.argv[1:],
                start_new_session=True
            )
    except ValueError:
        print(f"[WARNING] Impossible de lire les slots : {lines[0]!r}")
        connection.deconnexion()
        return

    # Si la map_size etait deja dans le meme paquet
    if len(lines) >= 2:
        map_size = lines[1]
    else:
        map_size = connection.receive()

    if map_size:
        print(f"[INFO] Map : {map_size.strip()}")

    # On passe les args au game pour que response_handler puisse
    # spawner les enfants apres confirmation Fork du serveur
    game.spawn_args = sys.argv[1:]

    game.start(connection, "auto", map_size, team_name=args.name)
    connection.deconnexion()

def main():
    args = parse_args()
    return connect(args)


if __name__ == "__main__":
    main()