/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** ProtocolHandler
*/

#pragma once
#include "Map.hpp"
#include "Player.hpp"
#include "Constants.hpp"
#include <list>
#include <array>
#include <functional>
#include <vector>
#include <algorithm>


namespace Zappy {
    class Client;
    class Core;

    class ProtocolHandler {
        public:
            ProtocolHandler(Client &client, Core &core);

            bool isReady() const { return _mapFullyLoaded; }
            bool isMapInitialized() const { return _mapInitialized; }

        private:
            bool safe_stoi(const std::string& str, int& value) const;
            bool parseId(const std::string& str, int& out) const;
            void handleMsz(const std::vector<std::string>& args); // Handle map size
            void handleSgt(const std::vector<std::string>& args); // Handle unit time request
            void handleBct(const std::vector<std::string>& args); // Handle tile content
            void handleTna(const std::vector<std::string>& args); // Handle team name
            void handlePnw(const std::vector<std::string>& args); // Handle player's connection
            void handlePpo(const std::vector<std::string>& args); // Handle player's position
            void handlePlv(const std::vector<std::string>& args); // Handle player's level
            void handlePin(const std::vector<std::string>& args); // Handle player's inventory
            void handlePdi(const std::vector<std::string>& args); // Handle player's death
            void handleEnw(const std::vector<std::string>& args); // Handle player lay an egg
            void handleEbo(const std::vector<std::string>& args); // Handle spawn in egg
            void handleEdi(const std::vector<std::string>& args); // Handle egg's death
            void handlePex(const std::vector<std::string>& args); // Handle eject
            void handlePbc(const std::vector<std::string>& args); // Handle broadcast
            void handlePic(const std::vector<std::string>& args); // Handle start of incantation
            void handlePie(const std::vector<std::string>& args); // Handle end of incantation
            void handlePfk(const std::vector<std::string>& args); // Handle fork
            void handlePdr(const std::vector<std::string>& args); // Handle resource drop
            void handlePgt(const std::vector<std::string>& args); // Handle resource pick
            void handleSeg(const std::vector<std::string>& args); // Handle end of game
            void handleSst(const std::vector<std::string>& args); // Handle time unit modification
            void handleSmg(const std::vector<std::string>& args); // Handle message from server
            void handleSuc(const std::vector<std::string>& args); // Handle suc
            void handleSbp(const std::vector<std::string>& args); // Handle sbp

            Client &_client;
            Core &_core;
            bool _mapInitialized;
            bool _mapFullyLoaded;
            int _expectedTiles;
            int _receivedTiles;
    };
}
