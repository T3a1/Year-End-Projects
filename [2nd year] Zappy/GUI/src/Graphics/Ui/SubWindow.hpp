/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** SubWindow
*/

#pragma once
#include "ISubWindowComponent.hpp"
#include <vector>
#include <memory>
#include <string>
#include <numeric>
#include <algorithm>

namespace Zappy {
    class SubWindow {
        public:
            SubWindow(float x1, float y1, float w1, float h1,
                float x2, float y2, float w2, float h2,
                float animationSpeed, int layerIndex);
            ~SubWindow() = default;
            SubWindow(SubWindow &&subwindow) = default;
            SubWindow &operator=(SubWindow &&subwindow) = delete;
            SubWindow(const SubWindow &subwindow) = delete;
            SubWindow &operator=(const SubWindow &subwindow) = delete;

            static SubWindow createNormalized(float x1, float y1, float w1, float h1,
                float x2, float y2, float w2, float h2,
                float animationSpeed, int layerIndex);

            Rectangle getRect() const { return _rect; }
            int getLayerIndex() const { return _layerIndex; }

            bool isVisible() const { return _visibility; }
            void setVisible(bool visible);
            void setTargetRect(Rectangle target, float speed);
            bool togglePosition();
            void update(float deltaTime);
            bool isAnimating() const { return _animationProgress < 1.f; }

            void addComponent(std::shared_ptr<ISubWindowComponent> component);
            void updateComponents(float deltaTime);
            void drawComponents() const;
            bool processMouseInput(const Vector2 &mousePos, bool mousePressed);
            void recomputeRects();

        private:
            Rectangle _rect;
            Rectangle _offscreenRect;
            Rectangle _visibleRect;
            Rectangle _startRect;
            Rectangle _targetRect;
            Rectangle _normalizedOffscreen;
            Rectangle _normalizedVisible;

            float _animationProgress;
            float _animationSpeed;
            int _layerIndex;
            bool _visibility;
            bool _hideWhenDone;
            std::vector<std::shared_ptr<ISubWindowComponent>> _components;
        };
}
