/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** ChatPanelHandler
*/

#include "ChatPanelHandler.hpp"
#include "SubWindow.hpp"
#include "TextureComponent.hpp"

namespace Zappy {

ChatPanelHandler::ChatPanelHandler()
{
}

void ChatPanelHandler::init(SubWindow& chatWindow, std::function<void(int)> onSelectPlayer,
    std::function<Color(const std::string&)> getTeamColor) {
    auto container = std::make_unique<ContainerComponent>(0.5f, 0.5f, 1.f, 1.f, 0);
    _container = std::move(container);

    _container->addChild(std::make_unique<TextureComponent>(
        PANEL_TEXTURE_PATH, 0.5f, 0.5f, 1.f, 1.f, 0
    ));

    auto info = std::make_unique<ChatInfoComponent>(onSelectPlayer, getTeamColor, 0.5f, 0.5f, 1.f, 1.f, 1);
    _chatInfo = std::ref(*info);
    _container->addChild(std::move(info));

    chatWindow.addComponent(std::move(_container));
}

void ChatPanelHandler::addBroadcast(int senderId, const std::string& teamName, const std::string& message) {
    if (_chatInfo)
        _chatInfo->get().addBroadcast(senderId, teamName, message);
}

}
