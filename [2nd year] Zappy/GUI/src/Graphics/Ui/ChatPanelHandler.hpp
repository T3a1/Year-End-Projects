/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** ChatPanelHandler
*/

#pragma once
#include "ContainerComponent.hpp"
#include "ChatInfoComponent.hpp"
#include "Button.hpp"
#include "ActionMap.hpp"
#include <optional>

namespace Zappy {
    class SubWindow;

    class ChatPanelHandler {
        public:
            ChatPanelHandler();

            void init(SubWindow& chatWindow, std::function<void(int)> onSelectPlayer,
                std::function<Color(const std::string&)> getTeamColor);
            void addBroadcast(int senderId, const std::string& teamName, const std::string& message);

        private:
            std::unique_ptr<ContainerComponent> _container;
            std::optional<std::reference_wrapper<ChatInfoComponent>> _chatInfo;
    };
}
