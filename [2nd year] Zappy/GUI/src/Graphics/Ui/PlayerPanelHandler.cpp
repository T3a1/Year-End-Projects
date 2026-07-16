/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** PlayerPanelHandler
*/

#include "PlayerPanelHandler.hpp"
#include "SubWindow.hpp"
#include "TextureComponent.hpp"

namespace Zappy {

PlayerPanelHandler::PlayerPanelHandler(ActionMap& actionMap)
    : _actionMap(actionMap)
{
}

void PlayerPanelHandler::init(SubWindow& playersWindow, std::function<const Player*()> getPlayer) {
    auto container = std::make_unique<ContainerComponent>(0.5f, 0.5f, 1.f, 1.f, 0);
    _container = std::move(container);

    _container->addChild(std::make_unique<TextureComponent>(
        PANEL_TEXTURE_PATH, 0.5f, 0.5f, 1.f, 1.f, 0
    ));

    auto info = std::make_unique<PlayerInfoComponent>(
        getPlayer, 0.5f, 0.5f, 1.f, 1.f, 2
    );
    _container->addChild(std::move(info));

    //auto tile = std::make_unique<TextureComponent>(
    //    INFO_TILE_TEXTURE_PATH, 0.5f, 0.45f, 0.6f, 0.15f, 1
    //);
    //_container->addChild(std::move(tile));

    auto leftArrow = std::make_unique<Button>(
        INFO_LEFT_ARROW_TEXTURE_PATH, playersWindow,
        Rectangle{0.25f, 0.35f, 0.1f, 0.075f},
        Rectangle{0.25f, 0.35f, 0.1f, 0.075f},
        1.f, 1.f, "prevPlayer", _actionMap, 3
    );
    _container->addChild(std::move(leftArrow));

    auto rightArrow = std::make_unique<Button>(
        INFO_RIGHT_ARROW_TEXTURE_PATH, playersWindow,
        Rectangle{0.75f, 0.35f, 0.1f, 0.075f},
        Rectangle{0.75f, 0.35f, 0.1f, 0.075f},
        1.f, 1.f, "nextPlayer", _actionMap, 3
    );
    _container->addChild(std::move(rightArrow));

    playersWindow.addComponent(std::move(_container));
}

}
