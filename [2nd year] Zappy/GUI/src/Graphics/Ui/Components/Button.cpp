/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** Button
*/

#include "Button.hpp"

namespace Zappy {

Button::Button(const std::string &texturePath, const SubWindow &parentWindow,
    Rectangle normalRect, Rectangle alternateRect, float animationProgress, float animationSpeed,
    const std::string &actionName, ActionMap &actionMap, int layer)
    : ASubWindowComponent(0.5f, 0.5f, 1.f, 1.f, layer), _parentWindow(parentWindow),
      _normalRect(normalRect), _alternateRect(alternateRect), _currentRect(normalRect),
      _startRect(normalRect), _targetRect(normalRect), _animationProgress(animationProgress),
      _animationSpeed(animationSpeed), _actionName(actionName), _actionMap(actionMap),
      _isVisible(true), _wasPressed(false), _hasTexture(!texturePath.empty()), _rectangleImage(WHITE, _currentRect, texturePath)
{

}

Button::~Button() {
}

void Button::update(float deltaTime) {
    if (!_isVisible)
        return;

    if (_animationProgress < 1.f) {
        _animationProgress += _animationSpeed * deltaTime;
        if (_animationProgress > 1.f)
            _animationProgress = 1.f;

        float time = _animationProgress * _animationProgress * (3.f - 2.f * _animationProgress);
        _currentRect.x = _startRect.x + (_targetRect.x - _startRect.x) * time;
        _currentRect.y = _startRect.y + (_targetRect.y - _startRect.y) * time;
        _currentRect.width = _startRect.width + (_targetRect.width - _startRect.width) * time;
        _currentRect.height = _startRect.height + (_targetRect.height - _startRect.height) * time;
    }
}

void Button::draw(const Rectangle &parentRect) const {
    
if (!_isVisible || !_hasTexture)
        return;

    Rectangle absoluteRect = getAbsoluteRect(parentRect);
    DrawTexturePro(_rectangleImage.getImageTexture().getTexture(),
        {0, 0, static_cast<float>(_rectangleImage.getImageTexture().getTexture().width), static_cast<float>(_rectangleImage.getImageTexture().getTexture().height)},
        absoluteRect, {0,0}, 0.f, WHITE
    );
}   

bool Button::onMouse(const Vector2 &mousePos, bool pressed) {
    if (!_isVisible)
        return false;

    Rectangle parentRect = _parentWindow.getRect();
    Rectangle absoluteRect = getAbsoluteRect(parentRect);
    bool inside = CheckCollisionPointRec(mousePos, absoluteRect);

    if (inside) {
        if (pressed && !_wasPressed) {
            _wasPressed = true;
            return true;
        }
        if (!pressed && _wasPressed) {
            _actionMap.trigger(_actionName);
            _wasPressed = false;
            return true;
        }
    } else
        _wasPressed = false;

    return false;
}

    bool Button::onMouse(const Vector2 &mousePos, bool pressed, const Rectangle &parentRect) {
        if (!_isVisible)
            return false;

        Rectangle absoluteRect = getAbsoluteRect(parentRect);
        bool inside = CheckCollisionPointRec(mousePos, absoluteRect);

        if (inside) {
            if (pressed && !_wasPressed) {
                _wasPressed = true;
                return true;
            }
            if (!pressed && _wasPressed) {
                _actionMap.trigger(_actionName);
                _wasPressed = false;
                return true;
            }
        } else
            _wasPressed = false;

        return false;
    }


void Button::togglePosition() {
    bool isAtNormalRect = (_targetRect.x == _normalRect.x && _targetRect.y == _normalRect.y);

    _startRect = _currentRect;
    _targetRect = isAtNormalRect ? _alternateRect : _normalRect;
    _animationProgress = 0.f;
}

Rectangle Button::getAbsoluteRect(const Rectangle &parentRect) const {
    float absoluteWidth = _currentRect.width * parentRect.width;
    float absoluteHeight = _currentRect.height * parentRect.height;

    return {
        parentRect.x + _currentRect.x * parentRect.width - absoluteWidth * 0.5f,
        parentRect.y + _currentRect.y * parentRect.height - absoluteHeight * 0.5f,
        absoluteWidth,
        absoluteHeight
    };
}

void Button::setTexture(const std::string &texturePath) {
    _rectangleImage.setTexture(texturePath);
    _hasTexture = !texturePath.empty();
}

}
