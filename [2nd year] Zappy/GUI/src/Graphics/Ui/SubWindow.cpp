/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** SubWindow
*/

#include "SubWindow.hpp"
#include "Errors.hpp"

namespace Zappy {

SubWindow::SubWindow(float x1, float y1, float w1, float h1,
    float x2, float y2, float w2, float h2,
    float animationSpeed, int layerIndex)
    : _rect{x1, y1, w1, h1},
    _offscreenRect{x1, y1, w1, h1},
    _visibleRect{x2, y2, w2, h2},
    _startRect{x1, y1, w1, h1},
    _targetRect{x1, y1, w1, h1},
    _animationProgress(1.f),
    _animationSpeed(animationSpeed),
    _layerIndex(layerIndex),
    _visibility(false),
    _hideWhenDone(false)
{
}

SubWindow SubWindow::createNormalized(float x1, float y1, float w1, float h1,
    float x2, float y2, float w2, float h2,
    float animationSpeed, int layerIndex)
{
    float screenWidth = static_cast<float>(GetScreenWidth());
    float screenHeight = static_cast<float>(GetScreenHeight());

    float offscreenWidth = w1 * screenWidth, offscreenHeight = h1 * screenHeight;
    float offscreenX = x1 * screenWidth - offscreenWidth * 0.5f;
    float offscreenY = y1 * screenHeight - offscreenHeight * 0.5f;

    float visibleWidth = w2 * screenWidth, visibleHeight = h2 * screenHeight;
    float visibleX = x2 * screenWidth - visibleWidth * 0.5f;
    float visibleY = y2 * screenHeight - visibleHeight * 0.5f;

    SubWindow newSubWindow(offscreenX, offscreenY, offscreenWidth, offscreenHeight,
        visibleX, visibleY, visibleWidth, visibleHeight, animationSpeed, layerIndex);

    newSubWindow._normalizedOffscreen = { x1, y1, w1, h1 };
    newSubWindow._normalizedVisible = { x2, y2, w2, h2 };
    return newSubWindow;
}

void SubWindow::setVisible(bool visible) {
    _visibility = visible;
}

void SubWindow::setTargetRect(Rectangle target, float speed) {
    _startRect = _rect;
    _targetRect = target;
    _animationProgress = 0.f;
    _animationSpeed = speed;
}

bool SubWindow::togglePosition() {
    bool isAtVisibleRect = (_targetRect.x == _visibleRect.x &&
        _targetRect.y == _visibleRect.y);
    _startRect = _rect;

    if (isAtVisibleRect) {
        _targetRect = _offscreenRect;
        _hideWhenDone = true;
    } else {
        _targetRect = _visibleRect;
        _hideWhenDone = false;
        _visibility = true;
    }

    _animationProgress = 0.f;
    return !_hideWhenDone;
}

void SubWindow::update(float deltaTime) {
    if (_animationProgress >= 1.f)
        return;
    _animationProgress += _animationSpeed * deltaTime;
    if (_animationProgress > 1.f)
        _animationProgress = 1.f;

    float time = _animationProgress * _animationProgress * (3.f - 2.f * _animationProgress);
    _rect.x = _startRect.x + (_targetRect.x - _startRect.x) * time;
    _rect.y = _startRect.y + (_targetRect.y - _startRect.y) * time;
    _rect.width = _startRect.width + (_targetRect.width - _startRect.width) * time;
    _rect.height = _startRect.height + (_targetRect.height - _startRect.height) * time;

    if (_animationProgress >= 1.f && _hideWhenDone)
        _visibility = false;
}

void SubWindow::addComponent(std::shared_ptr<ISubWindowComponent> component) {
    auto it = std::lower_bound(_components.begin(), _components.end(), component,
        [](const std::shared_ptr<ISubWindowComponent> &a,
           const std::shared_ptr<ISubWindowComponent> &b) {
            return a->getLayer() < b->getLayer();
        }
    );

    _components.insert(it, std::move(component));
}

void SubWindow::updateComponents(float deltaTime) {
    for (auto &component : _components)
        component->update(deltaTime);
}

void SubWindow::drawComponents() const {
    for (const auto &component : _components)
        component->draw(_rect);
}

bool SubWindow::processMouseInput(const Vector2 &mousePos, bool mousePressed)
{
    for (auto it = _components.rbegin(); it != _components.rend(); it++) {
        ISubWindowComponent &component = **it;
        if (!component.isVisible())
            continue;

        Rectangle absoluteRect = component.getAbsoluteRect(_rect);
        if (CheckCollisionPointRec(mousePos, absoluteRect)) {
            if (component.onMouse(mousePos, mousePressed, _rect))
                return true;
        }
    }
    return false;
}

void SubWindow::recomputeRects() {
    float screenWidth = static_cast<float>(GetScreenWidth());
    float screenHeight = static_cast<float>(GetScreenHeight());

    float offWidth = _normalizedOffscreen.width * screenWidth;
    float offHeight = _normalizedOffscreen.height * screenHeight;
    _offscreenRect = {
        _normalizedOffscreen.x * screenWidth - offWidth * 0.5f,
        _normalizedOffscreen.y * screenHeight - offHeight * 0.5f,
        offWidth, offHeight
    };

    float visWidth = _normalizedVisible.width * screenWidth;
    float visHeight = _normalizedVisible.height * screenHeight;
    _visibleRect = {
        _normalizedVisible.x * screenWidth - visWidth * 0.5f,
        _normalizedVisible.y * screenHeight - visHeight * 0.5f,
        visWidth, visHeight
    };

    if (_visibility) {
        _rect = _visibleRect;
        _targetRect = _visibleRect;
    } else {
        _rect = _offscreenRect;
        _targetRect = _offscreenRect;
    }
    _startRect = _rect;
    _animationProgress = 1.f;
}

}
    