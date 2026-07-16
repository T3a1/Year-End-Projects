/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** TilePanelHandler
*/

#include "TilePanelHandler.hpp"
#include "SubWindow.hpp"
#include "TextureComponent.hpp"

namespace Zappy {

TilePanelHandler::TilePanelHandler(ActionMap& actionMap)
    : _actionMap(actionMap)
{
}

void TilePanelHandler::init(SubWindow& tileWindow, std::function<const Tile*()> getTile) {
    auto container = std::make_unique<ContainerComponent>(0.5f, 0.5f, 1.f, 1.f, 0);
    _container = std::move(container);

    _container->addChild(std::make_unique<TextureComponent>(
        PANEL_TEXTURE_PATH, 0.5f, 0.5f, 1.f, 1.f, 0
    ));

    _container->addChild(std::make_unique<TileInfoComponent>(
        getTile, 0.5f, 0.5f, 1.f, 1.f, 1
    ));

    auto closeButton = std::make_unique<Button>(
        CLOSE_CROSS_BUTTON_TEXTURE_PATH, tileWindow,
        Rectangle{0.9f, 0.1f, 0.08f, 0.07f},
        Rectangle{0.9f, 0.1f, 0.08f, 0.07f},
        1.f, 1.f, "toggleTile", _actionMap, 2
    );
    _container->addChild(std::move(closeButton));

    tileWindow.addComponent(std::move(_container));
}

}
