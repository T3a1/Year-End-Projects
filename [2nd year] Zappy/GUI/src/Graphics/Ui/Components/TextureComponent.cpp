/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** TextureComponent
*/

#include "TextureComponent.hpp"
#include "Errors.hpp"

namespace Zappy {

TextureComponent::TextureComponent(const std::string &texturePath,
    float x, float y, float width, float height, int layer, int frameCount, int frameWidth, int frameHeight)
    : ASubWindowComponent(x, y, width, height, layer), _frameCount(frameCount), _currentFrame(0), _texture(Raylib::ImageTexture(texturePath))
{
    _frameWidth = (frameWidth  > 0) ? frameWidth : _texture.getTexture().width / frameCount;
    _frameHeight = (frameHeight > 0) ? frameHeight : _texture.getTexture().height;
}

TextureComponent::~TextureComponent() {

}

void TextureComponent::draw(const Rectangle &parentRect) const {
    Rectangle source = {
        static_cast<float>(_currentFrame * _frameWidth), 0.f,
        static_cast<float>(_frameWidth),
        static_cast<float>(_frameHeight)
    };

    DrawTexturePro(_texture.getTexture(), source, resolveRect(parentRect), { 0, 0 }, 0.f, WHITE);
}

}
