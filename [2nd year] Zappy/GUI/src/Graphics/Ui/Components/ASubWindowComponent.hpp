/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** ASubWindowComponent
*/

#pragma once
#include "ISubWindowComponent.hpp"


namespace Zappy {

class ASubWindowComponent : public ISubWindowComponent {
    public:
        ASubWindowComponent(float x = 0.5f, float y = 0.5f,
            float width = 1.f,  float height = 1.f, int layer = 0);
        virtual ~ASubWindowComponent() = default;

        int getLayer() const override { return _layer; }
        bool onMouse(const Vector2&, bool) override { return false;}
        bool onMouse(const Vector2&, bool, const Rectangle&) override { return false; }
        Rectangle getAbsoluteRect(const Rectangle &parentRect) const override;
        void setVisible(bool visible) override { _visible = visible; }
        bool isVisible() const override { return _visible; }

    protected:
        Rectangle resolveRect(const Rectangle& parentRect) const;

        float _x;
        float _y;
        float _width;
        float _height;
        int _layer;
        bool _visible;
};

}
