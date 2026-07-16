/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** OneShotComponent
*/

#include "OneshotComponent.hpp"

namespace Zappy {

OneshotComponent::OneshotComponent(const std::string &textureName, int frameCount, float fps,
    const std::string &triggerAction, ActionMap &actionMap,
    float x, float y,
    float width, float height, int layer, int frameWidth, int frameHeight)
    : TextureComponent(textureName, x, y, width, height, layer, frameCount, frameWidth, frameHeight),
      _fps(fps), _elapsed(0.f), _playing(false), _done(false)
{
    actionMap.registerAction(triggerAction, [this]() { trigger(); });
}

void OneshotComponent::trigger() {
    _currentFrame = 0;
    _elapsed = 0.f;
    _playing = true;
    _done = false;
}

void OneshotComponent::draw(const Rectangle &parentRect) const {
    if (!_playing)
        return;

    TextureComponent::draw(parentRect);
}

void OneshotComponent::update(float deltaTime) {
    if (!_playing)
        return;

    _elapsed += deltaTime;
    if (_elapsed >= 1.f / _fps) {
        _elapsed = 0.f;
        _currentFrame++;

        if (_currentFrame >= _frameCount) {
            _currentFrame = 0;
            _playing = false;
            _done = true;
        }
    }
}

}
