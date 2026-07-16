/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** ContainerComponent
*/

#include "ContainerComponent.hpp"

namespace Zappy {

ContainerComponent::ContainerComponent(float x, float y, float width, float height, int layer)
    : ASubWindowComponent(x, y, width, height, layer), _visible(true)
{
}

void ContainerComponent::update(float deltaTime) {
    if (!_visible)
        return;

    for (auto &child : _children)
        child->update(deltaTime);

    for (auto &child : _sharedChildren)
        child->update(deltaTime);
}

void ContainerComponent::draw(const Rectangle &parentRect) const {
    if (!_visible)
        return;

    Rectangle containerRect = resolveRect(parentRect);
    for (auto &child : _children)
        child->draw(containerRect);
    for (auto &child : _sharedChildren)
        child->draw(containerRect);
}

bool ContainerComponent::onMouse(const Vector2 &mousePos, bool pressed) {
    if (!_visible)
        return false;

    for (auto it = _children.rbegin(); it != _children.rend(); it++)
        if ((*it)->onMouse(mousePos, pressed))
            return true;
    for (auto it = _sharedChildren.rbegin(); it != _sharedChildren.rend(); it++)
        if ((*it)->onMouse(mousePos, pressed))
            return true;
    return false;
}

bool ContainerComponent::onMouse(const Vector2 &mousePos, bool pressed, const Rectangle &parentRect) {
    if (!_visible)
        return false;

    Rectangle containerRect = resolveRect(parentRect);

    for (auto it = _children.rbegin(); it != _children.rend(); it++)
        if ((*it)->onMouse(mousePos, pressed, containerRect))
            return true;
    for (auto it = _sharedChildren.rbegin(); it != _sharedChildren.rend(); it++)
        if ((*it)->onMouse(mousePos, pressed, containerRect))
            return true;
    return false;
}


void ContainerComponent::setVisible(bool visible) {
    _visible = visible;
}

bool ContainerComponent::isVisible() const {
    return _visible;
}

void ContainerComponent::clearChildren() {
    _children.clear();
    _sharedChildren.clear();
}

void ContainerComponent::addChild(std::unique_ptr<ISubWindowComponent> child) {
    _children.push_back(std::move(child));
}

void ContainerComponent::addSharedChild(std::shared_ptr<ISubWindowComponent> child) {
    _sharedChildren.push_back(child);
}
}
