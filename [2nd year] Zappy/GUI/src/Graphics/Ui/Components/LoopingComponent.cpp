/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** LoopingComponent
*/

#include "LoopingComponent.hpp"

namespace Zappy {

LoopingComponent::LoopingComponent(const std::string &texturePath, int frameCount, float fps,
    float x, float y, float width, float height, int layer, int frameWidth, int frameHeight)
    : TextureComponent(texturePath, x, y, width, height, layer, frameCount, frameWidth, frameHeight),
      _fps(fps), _elapsed(0.f)
{
}

void LoopingComponent::update(float deltaTime) {
    _elapsed += deltaTime;

    if (_elapsed >= 1.f / _fps) {
        _elapsed = 0.f;
        _currentFrame = (_currentFrame + 1) % _frameCount;
    }
}

}
