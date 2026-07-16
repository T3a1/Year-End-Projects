/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** TextComponent
*/

#pragma once
#include "ASubWindowComponent.hpp"
#include "TextFont.hpp"
#include <string>

namespace Zappy {
    class TextComponent : public ASubWindowComponent {
        public:
            TextComponent(const std::string &text, Raylib::TextFont &font,
                float fontSize, Color color,
                float x = 0.5f, float y = 0.5f, float width = 1.f, float height = 1.f,
                int layer = 0);
            ~TextComponent() override = default;

            void update([[maybe_unused]] float deltaTime) override {};
            void draw(const Rectangle &parentRect) const override;

            void setText(const std::string &text) { _text = text; }
            const std::string &getText() const { return _text; }

            void setColor(Color color) { _color = color; }
            void setFontSize(float fontSize) { _fontSize = fontSize; }

        private:
            std::string _text;
            Raylib::TextFont &_font;
            float _fontSize;
            float _spacing;
            Color _color;
    };
}