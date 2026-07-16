/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** TilePanelHandler
*/

#pragma once
#include "ContainerComponent.hpp"
#include "TileInfoComponent.hpp"
#include "Button.hpp"
#include "ActionMap.hpp"
#include "Tile.hpp"
#include <functional>

namespace Zappy {
    class SubWindow;

    class TilePanelHandler {
        public:
            TilePanelHandler(ActionMap& actionMap);
            void init(SubWindow& tileWindow, std::function<const Tile*()> getTile);

        private:
            ActionMap& _actionMap;
            std::unique_ptr<ContainerComponent> _container;
    };
}
