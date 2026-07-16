/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** PlayerPanelHandler
*/

#pragma once
#include "ContainerComponent.hpp"
#include "Button.hpp"
#include "ActionMap.hpp"
#include "PlayerInfoComponent.hpp"

namespace Zappy {
    class SubWindow;

    class PlayerPanelHandler {
        public:
            PlayerPanelHandler(ActionMap& actionMap);

            void init(SubWindow& playersWindow, std::function<const Player*()> getPlayer);

        private:
            ActionMap& _actionMap;
            std::unique_ptr<ContainerComponent> _container;
    };
}
