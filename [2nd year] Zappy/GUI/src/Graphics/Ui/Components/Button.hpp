/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** Button
*/

#pragma once
#include "ASubWindowComponent.hpp"
#include "SubWindow.hpp"
#include "ActionMap.hpp"
#include "Errors.hpp"
#include <string>
#include "RectangleImage.hpp"

namespace Zappy {
    class Button : public ASubWindowComponent {
        public:
            Button(const std::string &texturePath, const SubWindow &parentWindow,
                Rectangle normalRect, Rectangle alternateRect, float animationProgress,
                float animationSpeed, const std::string &actionName,
                ActionMap &actionMap, int layer = 0);
            ~Button() override;

            void update(float deltaTime) override;
            void draw(const Rectangle &parentRect) const override;
            bool onMouse(const Vector2 &mousePos, bool pressed) override;
            bool onMouse(const Vector2 &mousePos, bool pressed, const Rectangle &parentRect) override;

            void setVisible(bool visible) override { _isVisible = visible; }
            bool isVisible() const override { return _isVisible; }
            void togglePosition();
            Rectangle getAbsoluteRect(const Rectangle &parentRect) const override;

            void setTexture(const std::string &texturePath);

        private:
        
            const SubWindow &_parentWindow;
            Rectangle _normalRect;
            Rectangle _alternateRect;
            Rectangle _currentRect;
            Rectangle _startRect;
            Rectangle _targetRect;
            float _animationProgress;
            float _animationSpeed;
            std::string _actionName;
            ActionMap &_actionMap;
            bool _isVisible;
            bool _wasPressed;
            bool _hasTexture;
            Raylib::RectangleImage _rectangleImage;
    };
}
