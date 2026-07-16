/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** main
*/

#include "Core.hpp"
#include "Errors.hpp"
#include "menu.hpp"
#include <iostream>
#include <sstream>
#include "Parser.hpp"

int main(int argc, char **argv)
{
    try {

        Zappy::Parser parser;
        
        int result = parser.parseArguments(argc, argv);

        if (result == 0)
            return 0;
        
        std::string port = parser.getPort();
        std::string machine = parser.getMachine();
        float mapDensity = parser.getMapDensity();

        srand(static_cast<unsigned int>(time(nullptr)));

        SetTraceLogLevel(LOG_NONE);
        InitWindow(GetScreenWidth(), GetScreenHeight(), WINDOW_TITLE);
        ToggleFullscreen();
        SetTargetFPS(WINDOW_FPS);
        InitAudioDevice();


        bool shouldQuit = false;
        std::string finalPort;
        std::string finalMachine;
        {
            Zappy::Menu menu(port, machine);
            menu.display();

            if (menu.shouldQuit())
                shouldQuit = true;
            else {
                finalPort = menu.getPort();
                finalMachine = menu.getMachine();
            }
        }

        if (shouldQuit) {
            CloseAudioDevice();
            CloseWindow();
            return 0;
        }

        {
            Zappy::Core core(mapDensity);
            core.initConnection(finalMachine, std::stoi(finalPort));
            core.run();
            
        }
            
        CloseAudioDevice();
        CloseWindow();
        return 0;

    } catch (const Zappy::GuiError &error) {
        std::cerr << error.what() << '\n';
        return 84;
    } catch (const std::exception &error) {
        std::cerr << error.what() << '\n';
        return 84;
    }
}
