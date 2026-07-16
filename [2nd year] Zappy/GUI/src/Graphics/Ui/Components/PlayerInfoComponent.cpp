/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** PlayerInfoComponent
*/

#include "PlayerInfoComponent.hpp"
#include <raylib.h>
#include "algorithm"

namespace Zappy {

PlayerInfoComponent::PlayerInfoComponent( std::function<const Player*()> getPlayer,
    float x, float y, float width, float height, int layer)
    : ASubWindowComponent(x, y, width, height, layer), _getPlayer(getPlayer)
{
    for (const auto& [resourceName, iconPath] : _resourceIcons)
        _resourceTextures.emplace(resourceName, Raylib::ImageTexture(iconPath));

    _previewTexture = LoadRenderTexture(256, 256);

    _previewCamera.position = { 1.5f, -2.f, 1.5f };
    _previewCamera.target = { 0.0f, 0.0f, 0.0f };
    _previewCamera.up = { 0.0f, 1.0f, 0.0f };
    _previewCamera.fovy = 45.0f;
    _previewCamera.projection = CAMERA_PERSPECTIVE;
}

PlayerInfoComponent::~PlayerInfoComponent() {
    if (_previewTexture.id != 0)
        UnloadRenderTexture(_previewTexture);

}

void PlayerInfoComponent::renderPlayerPreview(const Player& player) const {
    const Model& model = player.getModel();
    if (model.meshCount == 0)
        return;

    BeginTextureMode(_previewTexture);

        ClearBackground(BLANK);
        BeginMode3D(_previewCamera);
    
            float previewScale = 0.005f;
            DrawModelEx(model, (Vector3){0,0,0}, (Vector3){0,1,0}, 0.0,
                (Vector3){previewScale, -previewScale, previewScale}, WHITE);

        EndMode3D();
    
    EndTextureMode();
}

void PlayerInfoComponent::draw( const Rectangle& parentRect) const {
    const Player* player = _getPlayer();

    const float screenH = static_cast<float>(GetScreenHeight());
    const float screenW = static_cast<float>(GetScreenWidth());
    const float screenScale = std::clamp(std::min(screenH / 900.0f, screenW / 1600.0f), 0.5f, 1.5f);
    int fontSize = static_cast<int>(std::min(screenH, screenW) * 0.016f * screenScale);

    if (!player) {
        fontSize = std::clamp(fontSize, 8, 30);
        DrawText("No player selected", static_cast<int>(parentRect.x) + static_cast<int>(parentRect.width * 0.2f),
            static_cast<int>(parentRect.y) + static_cast<int>(parentRect.height * 0.1f), 25, GRAY);
        return;
    }

    renderPlayerPreview(*player);

    Rectangle rect = resolveRect(parentRect);
    auto getPos = [&](float newX, float newY) -> Vector2 {
        return { rect.x + newX * rect.width, rect.y + newY * rect.height };
    };

    fontSize = std::clamp(fontSize, 8, static_cast<int>(rect.height * 0.04f));
    Color textColor = DARKGRAY;

    auto drawCentered = [&](const std::string& text, Vector2 center, int size, Color color) {
        int width = MeasureText(text.c_str(), size);
        int height = size;
        int x = static_cast<int>(center.x - width / 2);
        int y = static_cast<int>(center.y - height / 2);
        DrawText(text.c_str(), x, y, size, color);
    };
    drawCentered("ID: " + std::to_string(player->getId()), getPos(0.5f, 0.1f), fontSize, textColor);
    drawCentered("Level: " + std::to_string(player->getLevel()), getPos(0.5f, 0.12f), fontSize, textColor);
    drawCentered("Team: " + player->getTeam(), getPos(0.5f, 0.14f), fontSize, textColor);

    float previewSize = 0.7f;
    Vector2 previewPos = getPos(0.5f - previewSize / 2, 0.15f);
    Rectangle dest = { previewPos.x, previewPos.y,
        previewSize * rect.width, previewSize * rect.height };

    DrawTexturePro(_previewTexture.texture, {0, 0,
        static_cast<float>(_previewTexture.texture.width), static_cast<float>(_previewTexture.texture.height)},
        dest, {0,0}, 0.f, WHITE
    );

    const std::array<int, RESOURCE_COUNT>& inventory = player->getInventory();
    const std::array<std::string, RESOURCE_COUNT> resourceNames = {
        "Food", "Linemate", "Deraumere", "Sibur",
        "Mendiane", "Phiras", "Thystame"
    };

    float rowHeight = 0.05f;
    float startY = 0.55f;
    float iconX = 0.75f;
    float iconWidth = 0.1f;
    float iconHeight = 0.075f;

    for (int i = 0; i < RESOURCE_COUNT; i++) {
        Vector2 position = getPos(0.2f, startY + i * rowHeight);
        std::string resourceText = resourceNames[i] + ": " +
            (inventory[i] > 99 ? "99+" : std::to_string(inventory[i]));
        DrawText(resourceText.c_str(), static_cast<int>(position.x),
            static_cast<int>(position.y), fontSize, textColor);

        if (_resourceTextures.at(resourceNames[i]).getTexture().id != 0) {
            Vector2 iconPos = getPos(iconX - iconWidth / 2,
                startY + i * rowHeight - iconHeight / 2);
            Rectangle dest = {
                iconPos.x, iconPos.y,
                iconWidth * rect.width,
                iconHeight * rect.height
            };

            DrawTexturePro(_resourceTextures.at(resourceNames[i]).getTexture(),
                {0, 0, static_cast<float>(_resourceTextures.at(resourceNames[i]).getTexture().width),
                static_cast<float>(_resourceTextures.at(resourceNames[i]).getTexture().height)},
                dest, {0,0}, 0.f, WHITE);
        }
    }
}

}
