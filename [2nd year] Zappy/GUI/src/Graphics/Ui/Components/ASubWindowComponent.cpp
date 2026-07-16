/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** ASubWindowComponent
*/

#include "ASubWindowComponent.hpp"

namespace Zappy {

ASubWindowComponent::ASubWindowComponent(float x, float y, float width, float height, int layer)
    : _x(x), _y(y), _width(width), _height(height), _layer(layer), _visible(true)
{
}

Rectangle ASubWindowComponent::resolveRect(const Rectangle& parentRect) const {
    float absoluteWidth = _width * parentRect.width;
    float absoluteHeight = _height * parentRect.height;

    return {
        parentRect.x + _x * parentRect.width - absoluteWidth * 0.5f,
        parentRect.y + _y * parentRect.height - absoluteHeight * 0.5f,
        absoluteWidth,
        absoluteHeight
    };
}

Rectangle ASubWindowComponent::getAbsoluteRect(const Rectangle &parentRect) const {
    return resolveRect(parentRect);
}

}
