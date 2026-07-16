/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** ISubWindowComponent
*/

#pragma once
#include <raylib.h>

namespace Zappy {
    class RaylibEngine;

    class ISubWindowComponent {
        public:
            virtual ~ISubWindowComponent() = default;

            virtual void update([[maybe_unused]]float deltaTime) = 0;
            virtual void draw(const Rectangle& parentRect) const = 0;
            virtual int getLayer() const = 0;
        
            virtual bool onMouse(const Vector2&, bool) = 0;
            virtual bool onMouse(const Vector2&, bool, const Rectangle &parentRect) = 0;
            virtual Rectangle getAbsoluteRect(const Rectangle &parentRect) const = 0;

            virtual void setVisible(bool visible) = 0;
            virtual bool isVisible() const = 0;
    };
}
