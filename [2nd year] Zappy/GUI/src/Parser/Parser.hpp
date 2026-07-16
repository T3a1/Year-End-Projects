/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** Parser
*/

#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <string>
#include <sstream>
#include "iostream"
#include "Errors.hpp"

namespace Zappy {
    class Parser {
        public:
            Parser();
            ~Parser();

            int parseArguments(int argc, char **argv);

            std::string getPort() const { return _port; }
            std::string getMachine() const { return _machine; }
            float getMapDensity() const { return _mapDensity; }
            bool isRunningInTTY() const { return getenv("DISPLAY") != nullptr; }

        protected:
        private:

        std::string _port;
        std::string _machine;
        float _mapDensity;
    };
}

#endif /* !PARSER_HPP_ */
