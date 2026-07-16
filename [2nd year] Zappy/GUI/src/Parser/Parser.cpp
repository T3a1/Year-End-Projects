/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** Parser
*/

#include "Parser.hpp"
#include <unistd.h>

namespace Zappy {

    Parser::Parser()
    {
        _port = "X";
        _machine = "X";
        _mapDensity = 0.01f;
    }

    Parser::~Parser()
    {
    }
    
    int Parser::parseArguments(int argc, char **argv)
    {
        if (!isRunningInTTY())
            throw Zappy::GuiError("GUI must be run in a graphical environment");

        if (argc == 2 && argv[1] == std::string("--help")) {
            std::cout << "USAGE: ./zappy_gui -p port -h machine [-d density]" << std::endl;
            return 0;
        }

        if (argc == 1)
            return 1;

        if (argc == 3 && argv[1] == std::string("-d")) {
            float densityValue = std::stof(argv[2]);

            if (densityValue < 0.0f || densityValue > 1.0f)
                throw Zappy::GuiError("Map density must be between 0.0 and 1.0.");

            _mapDensity = densityValue;
            return 1;
        }


        if ((argc == 5 || argc == 7) && argv[1] == std::string("-p") && argv[3] == std::string("-h")) {
            int portValue = std::stoi(argv[2]);

            if (portValue < 0 || portValue > 65535)
                throw Zappy::GuiError("Port number must be between 0 and 65535.");

            std::string ip = argv[4];
            std::stringstream ss(ip);
            std::string part;
            int count = 0;

            while (std::getline(ss, part, '.')) {
                if (part.empty())
                    throw Zappy::GuiError("Ip address is not valid.");

                for (char c : part)
                    if (!std::isdigit(c))
                        throw Zappy::GuiError("Ip address is not valid.");

                int value = std::stoi(part);
                if (value < 0 || value > 255)
                    throw Zappy::GuiError("Ip address is not valid.");

                count++;
            }

            if (count != 4)
                throw Zappy::GuiError("Ip address is not valid.");

            _port = argv[2];
            _machine = argv[4];
            if (argc == 7 && argv[5] == std::string("-d")) {
                float densityValue = std::stof(argv[6]);

                if (densityValue < 0.0f || densityValue > 1.0f)
                    throw Zappy::GuiError("Map density must be between 0.0 and 1.0.");

                _mapDensity = densityValue;
            }
            return 1;
        }

        throw Zappy::GuiError("Invalid number of arguments. Use --help for usage information.");
    }
}

