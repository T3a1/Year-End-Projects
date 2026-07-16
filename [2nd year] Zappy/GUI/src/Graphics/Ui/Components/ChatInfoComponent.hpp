/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** ChatInfoComponent
*/

#pragma once
#include "ASubWindowComponent.hpp"
#include "Constants.hpp"
#include <string>
#include <deque>
#include <functional>

namespace Zappy {
    struct BroadcastInfo {
        int senderId;
        std::string teamName;
        std::string message;
    };

    class ChatInfoComponent : public ASubWindowComponent {
        public:
            ChatInfoComponent(std::function<void(int)> onSelectPlayer, 
                std::function<Color(const std::string&)> getTeamColor, float x = 0.5f, float y = 0.5f,
                float width = 1.f, float height = 1.f, int layer = 0);

            void addBroadcast(int senderId, const std::string& teamName, const std::string& message);
            void draw(const Rectangle& parentRect) const override;
            void update(float) override {}

        private:
            std::deque<BroadcastInfo> _broadcasts;
            std::function<void(int)> _onSelectPlayer;
            std::function<Color(const std::string&)> _getTeamColor;
            mutable std::vector<std::pair<Rectangle, int>> _broadcastRects;
    };
}
