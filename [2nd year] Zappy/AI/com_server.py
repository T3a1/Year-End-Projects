#
# EPITECH PROJECT, 2026
# Zappy
# File description:
# com_server
#

import socket


class COM_SERVER:
    def __init__(self, ip="127.0.0.1", port=4444, timeout=None, encoding="utf-8"):
        self.ip = ip
        self.port = port
        self.timeout = timeout
        self.encoding = encoding
        self.socket = None
        self.buffer = ""

    def connexion(self, ip=None, port=None, timeout=None):
        target_ip = ip if ip is not None else self.ip
        target_port = port if port is not None else self.port
        target_timeout = timeout if timeout is not None else self.timeout

        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        if target_timeout is not None:
            self.socket.settimeout(target_timeout)

        try:
            self.socket.connect((target_ip, target_port))
        except ConnectionRefusedError:
            print(f"[ERREUR] Connexion refusee par {target_ip}:{target_port} "
                  f"-- le serveur Zappy n'est pas lance ou ecoute sur un autre port.")
            self.socket = None
            return False
        except socket.gaierror:
            print(f"[ERREUR] Impossible de resoudre l'adresse '{target_ip}' "
                  f"-- machine inconnue ou nom invalide.")
            self.socket = None
            return False
        except socket.timeout:
            print(f"[ERREUR] Timeout en tentant de joindre {target_ip}:{target_port}.")
            self.socket = None
            return False
        except OSError as e:
            print(f"[ERREUR] Connexion impossible a {target_ip}:{target_port} ({e}).")
            self.socket = None
            return False

        self.ip = target_ip
        self.port = target_port
        self.timeout = target_timeout
        return True

    def deconnexion(self):
        if self.socket is not None:
            try:
                self.socket.shutdown(socket.SHUT_RDWR)
            except OSError:
                pass
            self.socket.close()
            self.socket = None

    def receive(self):
        if self.socket is None:
            return None

        while "\n" not in self.buffer:
            try:
                data = self.socket.recv(1024)
                if not data:
                    self.deconnexion()
                    return None
                self.buffer += data.decode(self.encoding)
            except (BlockingIOError, socket.timeout):
                break
            except OSError:
                self.deconnexion()
                return None

        if "\n" not in self.buffer:
            return ""

        line, self.buffer = self.buffer.split("\n", 1)
        line_with_nl = line + "\n"

        if not (line.startswith("[") or line in ("ok", "ko")) and line != "":
            print(f"[SERVEUR] {line}")

        if line == "dead":
            self.deconnexion()

        return line_with_nl

    def send(self, message):
        if self.socket is None:
            return False
        try:
            if not message.endswith("\n"):
                message += "\n"
            self.socket.sendall(message.encode(self.encoding))
            return True
        except (BrokenPipeError, ConnectionResetError, OSError):
            self.deconnexion()
            return False
