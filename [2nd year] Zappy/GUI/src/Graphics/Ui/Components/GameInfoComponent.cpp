/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** GameInfoComponent
*/

#include "GameInfoComponent.hpp"
#include "algorithm"

namespace Zappy {

GameInfoComponent::GameInfoComponent(std::function<GameInfo()> getInfo, Raylib::TextFont &font,
    float x, float y, float width, float height, int layer)
    : ASubWindowComponent(x, y, width, height, layer), _getInfo(getInfo), _font(font)
{
}

void GameInfoComponent::draw(const Rectangle& parentRect) const {
    Rectangle rect = resolveRect(parentRect);
    GameInfo info = _getInfo();

    const float screenH = static_cast<float>(GetScreenHeight());
    const float screenW = static_cast<float>(GetScreenWidth());
    const float screenScale = std::clamp(std::min(screenH / 900.0f, screenW / 1600.0f), 0.5f, 1.5f);

    const float leftX   = rect.x + rect.width * 0.07f;
    const float splitAt = 0.4f;

    std::vector<std::string> leftLines;
    leftLines.push_back("=== Game Info ===");
    leftLines.push_back("Map: " + std::to_string(info.mapWidth) + "x" + std::to_string(info.mapHeight));
    leftLines.push_back("Dead: " + std::to_string(info.deadPlayers));
    leftLines.push_back("Resources:");
    std::vector<std::string> stoneNames = {"Food", "Linemate", "Deraumere", "Sibur", "Mendiane", "Phiras", "Thystame"};
    int totalAllRes = 0;
    for (int i = 0; i < 7; i++) {
        totalAllRes += info.totalResources[i];
        leftLines.push_back("  " + stoneNames[i] + ": " + std::to_string(info.totalResources[i]));
    }
    leftLines.push_back("  Total: " + std::to_string(totalAllRes));

    const float leftStartY = rect.y + rect.height * 0.07f;
    const float leftEndY = rect.y + rect.height * 0.80f;
    const float leftStep = (leftEndY - leftStartY) / static_cast<float>(leftLines.size());
    int leftFontSize = static_cast<int>(screenH * 0.012f * screenScale);
    leftFontSize = std::clamp(leftFontSize, 6, static_cast<int>(leftStep * 0.70f));

    for (size_t i = 0; i < leftLines.size(); i++) {
        float y = leftStartY + i * leftStep + (leftStep - leftFontSize) * 0.5f;
        DrawTextEx(_font.getFont(), leftLines[i].c_str(), { leftX, y },
            static_cast<float>(leftFontSize), 1.0f, BLACK);
    }

    DrawLineEx(
        { rect.x + rect.width * splitAt, rect.y + rect.height * 0.01f },
        { rect.x + rect.width * splitAt, rect.y + rect.height * 0.92f },
        1.f, { 0, 0, 0, 60 }
    );

    const float rightX = rect.x + rect.width * (splitAt + 0.03f);
    const float rightEndY = rect.y + rect.height * 0.92f;
    const int maxSlots = 20;
    const float rightTopMargin = rect.height * 0.01f;
    const float slotH = (rightEndY - rect.y - rightTopMargin) / static_cast<float>(maxSlots);

    int teamFontSize = static_cast<int>(screenH * 0.016f * screenScale);
    teamFontSize = std::clamp(teamFontSize, 6, static_cast<int>(slotH * 0.44f));
    int lvlFontSize = std::clamp(static_cast<int>(teamFontSize * 0.85f), 4, teamFontSize);

    for (int team = 0; team < static_cast<int>(info.teamNames.size()) && team < maxSlots; team++) {
        const std::string& name = info.teamNames[team];
        int playerCount = info.playerCountPerTeam.count(name) ? info.playerCountPerTeam.at(name) : 0;
        float slotY = rect.y + rightTopMargin + team * slotH;
        float nameY = slotY;
        float levelY = slotY + (slotH * 0.45f);

        std::string nameLine = name + ": " + std::to_string(playerCount) + "p";
        DrawTextEx(_font.getFont(), nameLine.c_str(), { rightX, nameY },
            static_cast<float>(teamFontSize), 1.0f, BLACK);

        std::string levelLine;
        for (int level = 1; level <= 8; level++) {
            int count = 0;
            auto teamIt = info.playersPerLevelPerTeam.find(name);
            if (teamIt != info.playersPerLevelPerTeam.end()) {
                auto levelIt = teamIt->second.find(level);
                if (levelIt != teamIt->second.end())
                    count = levelIt->second;
            }
            levelLine += "l" + std::to_string(level) + ":" + std::to_string(count) + " ";
        }

        DrawTextEx(_font.getFont(), levelLine.c_str(), { rightX, levelY },
            static_cast<float>(lvlFontSize), 1.0f, DARKGRAY);
    }
}

}
