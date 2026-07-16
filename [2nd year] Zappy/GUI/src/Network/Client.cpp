/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** Client
*/

#include "Client.hpp"
#include "Errors.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <cstring>
#include <iostream>
#include <sstream>

namespace Zappy {

Client::Client(const std::string &host, int port)
    : _connected(false), _sockfd(-1), _host(host)
{
    memset(&_serverAddress, 0, sizeof(_serverAddress));
    _serverAddress.sin_family = AF_INET;
    _serverAddress.sin_port = htons(port);
}

Client::~Client() {
    disconnect();
}

void Client::setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);

    if (flags == -1)
        throw NetworkError("NetworkError: fcntl F_GETFL failed");
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
        throw NetworkError("NetworkError: fcntl F_SETFL failed");
}

bool Client::connect() {
    if (_connected)
        return true;

    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd < 0)
        throw NetworkError("NetworkError: socket creation failed");

    if (inet_pton(AF_INET, _host.c_str(), &_serverAddress.sin_addr) <= 0)
        throw NetworkError("NetworkError: invalid IP address");

    if (::connect(_sockfd, (struct sockaddr*)&_serverAddress, sizeof(_serverAddress)) < 0) {
        close(_sockfd);
        _sockfd = -1;
        return false;
    }

    setNonBlocking(_sockfd);
    _connected = true;
    return true;
}

void Client::disconnect() {
    if (_sockfd != -1) {
        close(_sockfd);
        _sockfd = -1;
    }
    _connected = false;
}

void Client::send(const std::string &command) {
    if (!_connected)
        throw NetworkError("NetworkError: not connected");

    std::string toSend = command + "\n";
    size_t total = 0;
    while (total < toSend.size()) {
        ssize_t sent = ::send(_sockfd, toSend.c_str() + total, toSend.size() - total, 0);

        if (sent <= 0)
            throw NetworkError("NetworkError: send failed or connection closed");
        total += sent;
    }
}

void Client::update() {
    if (!_connected)
        return;

    struct pollfd pollFd;
    pollFd.fd = _sockfd;
    pollFd.events = POLLIN;
    pollFd.revents = 0;

    int pollReturn = poll(&pollFd, 1, 0);
    if (pollReturn <= 0)
        return;

    if (pollFd.revents & POLLIN) {
        char buffer[4096];

        ssize_t received = recv(_sockfd, buffer, sizeof(buffer) - 1, 0);
        if (received <= 0) {
            disconnect();
            return;
        }

        buffer[received] = '\0';
        _buffer += buffer;
    }

    size_t pos;
    while ((pos = _buffer.find('\n')) != std::string::npos) {
        std::string line = _buffer.substr(0, pos);
        _buffer.erase(0, pos + 1);

        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (line.empty())
            continue;

        parseLine(line);
    }
}

void Client::registerHandler(const std::string& command,
    std::function<void(const std::vector<std::string>&)> handler) {
    _handlers[command] = handler;
}

void Client::parseLine(const std::string& line) {
    std::istringstream iss(line);
    std::string cmd;
    iss >> cmd;
    if (cmd.empty())
        return;

    std::vector<std::string> args;
    std::string arg;
    while (iss >> arg)
        args.push_back(arg);

    auto it = _handlers.find(cmd);
    if (it != _handlers.end())
        it->second(args);
    else
        std::cerr << "Unknown command from server: " << cmd << std::endl;
}

}
