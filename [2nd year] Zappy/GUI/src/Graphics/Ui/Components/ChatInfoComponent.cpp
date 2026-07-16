/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** ChatInfoComponent
*/

#include "ChatInfoComponent.hpp"
#include <raylib.h>
#include <algorithm>
#include <iostream>

namespace Zappy {

ChatInfoComponent::ChatInfoComponent(std::function<void(int)> onSelectPlayer,
    std::function<Color(const std::string&)> getTeamColor, float x, float y,
    float width, float height, int layer)
    : ASubWindowComponent(x, y, width, height, layer), _onSelectPlayer(onSelectPlayer), _getTeamColor(getTeamColor)
{
}

void ChatInfoComponent::addBroadcast(int senderId, const std::string& teamName, const std::string& message) {
    if (_broadcasts.size() >= MAX_BROADCASTS_IN_PANEL)
        _broadcasts.pop_front();
    _broadcasts.emplace_back(BroadcastInfo{ senderId, teamName, message });
}

void ChatInfoComponent::draw(const Rectangle& parentRect) const {
    _broadcastRects.clear();
    Rectangle rect = resolveRect(parentRect);

    const float screenH = static_cast<float>(GetScreenHeight());
    const float screenW = static_cast<float>(GetScreenWidth());

    const float horizontalPadding = rect.width * 0.15f;
    const float lineWidth = rect.width * 0.7f;
    const float lineX = rect.x + horizontalPadding;
    const int maxRows = MAX_BROADCASTS_IN_PANEL + 1;

    const float screenScale = std::clamp(std::min(screenH / 900.0f, screenW / 1600.0f), 0.5f, 1.5f);
    const float rectHeight = (rect.height / maxRows) * 0.45f;
    const float gap = rectHeight * 0.55f;
    const float rowHeight = rectHeight + gap;
    const float startY = rect.height * 0.12f;

    int fontSize = static_cast<int>(std::min(screenH, screenW) * 0.016f * screenScale);
    fontSize = std::clamp(fontSize, 8, static_cast<int>(rectHeight * 0.42f));

    DrawText("Broadcasts:", static_cast<int>(rect.x + rect.width * 0.05f),
        static_cast<int>(rect.y + rect.height * 0.08f), fontSize, DARKGRAY);

    for (size_t i = 0; i < _broadcasts.size(); i++) {
        const auto& info = _broadcasts[i];
        float yPos = startY + i * rowHeight;

        Color bgColor = { 0, 0, 0, 20 };
        Rectangle backgroundRect = { lineX, rect.y + yPos, lineWidth, rectHeight };
        DrawRectangleRec(backgroundRect, bgColor);
        _broadcastRects.emplace_back(backgroundRect, info.senderId);

        float textX = lineX + 8.0f;
        float textY1 = rect.y + yPos + 4.0f;
        float textY2 = textY1 + fontSize + 2;

        std::string prefix = "Player " + std::to_string(info.senderId) + " - Team: ";
        int prefixWidth = MeasureText(prefix.c_str(), fontSize);

        DrawText(prefix.c_str(), static_cast<int>(textX), static_cast<int>(textY1), fontSize, WHITE);

        Color teamColor = _getTeamColor ? _getTeamColor(info.teamName) : WHITE;
        DrawText(info.teamName.c_str(), static_cast<int>(textX + prefixWidth), static_cast<int>(textY1), fontSize, teamColor);

        std::string messageText = "Said: \"" + info.message + "\"";
        DrawText(messageText.c_str(), static_cast<int>(textX), static_cast<int>(textY2), fontSize, WHITE);
    }
}

}
