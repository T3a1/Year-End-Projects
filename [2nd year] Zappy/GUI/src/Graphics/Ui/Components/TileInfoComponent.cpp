/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** TileInfoComponent
*/

#include "TileInfoComponent.hpp"
#include <raylib.h>
#include "algorithm"

namespace Zappy {

TileInfoComponent::TileInfoComponent(std::function<const Tile*()> getTile,
    float x, float y, float width, float height, int layer)
    : ASubWindowComponent(x, y, width, height, layer), _getTile(getTile)
{
    for (const auto& [resourceName, iconPath] : _resourceIcons)
        _resourceTextures.emplace(resourceName, Raylib::ImageTexture(iconPath));
}

TileInfoComponent::~TileInfoComponent()
{
}

void TileInfoComponent::draw(const Rectangle& parentRect) const {
    const Tile* tile = _getTile();
    if (!tile) {
        DrawText("No tile selected", static_cast<int>(parentRect.x) + static_cast<int>(parentRect.width * 0.2f),
            static_cast<int>(parentRect.y) + static_cast<int>(parentRect.height * 0.1f), 25, GRAY);
        return;
    }

    Rectangle rect = resolveRect(parentRect);
    auto getPos = [&](float nx, float ny) -> Vector2 {
        return { rect.x + nx * rect.width, rect.y + ny * rect.height };
    };

    const std::array<std::string, RESOURCE_COUNT> resourceNames = {
        "Food", "Linemate", "Deraumere", "Sibur",
        "Mendiane", "Phiras", "Thystame"
    };

    std::array<int, RESOURCE_COUNT> resources;
    resources[0] = tile->getFoodCount();

    auto stoneCounts = tile->getStoneCounts();
    for (int i = 0; i < 6; i++) {
        StoneType type = static_cast<StoneType>(i);
        resources[i + 1] = stoneCounts.count(type) ? stoneCounts.at(type) : 0;
    }

    float rowHeight = 0.1f;
    float startY = 0.2f;
    float iconSize = rect.width * 0.15f;
    float textX = 0.2f;
    float iconX = 0.75f;
    const float screenH = static_cast<float>(GetScreenHeight());
    const float screenW = static_cast<float>(GetScreenWidth());
    const float screenScale = std::clamp(std::min(screenH / 900.0f, screenW / 1600.0f), 0.5f, 1.5f);
    int fontSize = static_cast<int>(std::min(screenH, screenW) * 0.016f * screenScale);
    fontSize = std::clamp(fontSize, 8, static_cast<int>(rect.height * 0.04f));

    for (int i = 0; i < RESOURCE_COUNT; i++) {
        Vector2 position = getPos(textX, startY + i * rowHeight);
        std::string text = resourceNames[i] + ": " + std::to_string(resources[i]);
        DrawText(text.c_str(), static_cast<int>(position.x),
            static_cast<int>(position.y), fontSize, DARKGRAY);

        if (_resourceTextures.at(resourceNames[i]).getTexture().id != 0) {
            Vector2 iconPos = getPos(iconX - iconSize / (2 * rect.width),
                startY + i * rowHeight - iconSize / (2 * rect.height));
            Rectangle dest = {
                iconPos.x, iconPos.y,
                iconSize, iconSize
            };

            DrawTexturePro(_resourceTextures.at(resourceNames[i]).getTexture(),
                {0, 0, static_cast<float>(_resourceTextures.at(resourceNames[i]).getTexture().width),
                static_cast<float>(_resourceTextures.at(resourceNames[i]).getTexture().height)},
                dest, {0,0}, 0, WHITE);
        }
    }
}

}
