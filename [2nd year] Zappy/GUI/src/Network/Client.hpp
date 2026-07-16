/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** Client
*/

#pragma once
#include "Constants.hpp"
#include <string>
#include <poll.h>
#include <netinet/in.h>
#include <functional>

namespace Zappy {
    class Client {
        public:
            Client(const std::string &host, int port);
            ~Client();

            bool connect();
            void disconnect();
            void send(const std::string &command);
            void update();
            bool isConnected() const { return _connected; }
            void registerHandler(const std::string& command,
                std::function<void(const std::vector<std::string>&)> handler);

        private:
            void setNonBlocking(int fd);
            void parseLine(const std::string& line);

            bool _connected;
            int _sockfd;
            struct sockaddr_in _serverAddress;
            std::string _host;
            std::string _buffer;
            std::unordered_map<std::string, std::function<void(const std::vector<std::string>&)>> _handlers;
    };
}
