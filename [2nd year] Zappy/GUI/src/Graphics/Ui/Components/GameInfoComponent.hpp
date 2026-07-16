/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** GameInfoComponent
*/

#pragma once
#include "ASubWindowComponent.hpp"
#include "Constants.hpp"
#include "TextFont.hpp"
#include <functional>
#include <string>

namespace Zappy {
    struct GameInfo {
        int mapWidth = 0;
        int mapHeight = 0;
        int teamCount = 0;
        int deadPlayers = 0;
        std::vector<std::string> teamNames;
        std::map<std::string, int> playerCountPerTeam;
        std::map<std::string, std::map<int, int>> playersPerLevelPerTeam;
        std::array<int, RESOURCE_COUNT> totalResources;
        std::array<int, 9> playersPerLevel;
    };

    class GameInfoComponent : public ASubWindowComponent {
        public:
            GameInfoComponent(std::function<GameInfo()> getInfo, Raylib::TextFont &font,
                float x = 0.5f, float y = 0.5f, float width = 1.f, float height = 1.f, int layer = 0);

            void draw(const Rectangle& parentRect) const override;
            void update(float) override {}
            bool onMouse(const Vector2&, bool) override { return false; }

        private:
            std::function<GameInfo()> _getInfo;
            Raylib::TextFont &_font;
    };
}
