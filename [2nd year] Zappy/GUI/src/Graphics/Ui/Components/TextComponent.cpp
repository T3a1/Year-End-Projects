/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** TextComponent
*/

#include "TextComponent.hpp"
#include "Constants.hpp"

namespace Zappy {

TextComponent::TextComponent(const std::string &text, Raylib::TextFont &font,
    float fontSize, Color color, float x, float y, float width, float height, int layer)
    : ASubWindowComponent(x, y, width, height, layer),
      _text(text), _font(font), _fontSize(fontSize), _spacing(1.f), _color(color)
{
}

void TextComponent::draw(const Rectangle &parentRect) const {
    Rectangle box = resolveRect(parentRect);

    float scaleX = static_cast<float>(GetScreenWidth())  / WINDOW_WIDTH;
    float scaleY = static_cast<float>(GetScreenHeight()) / WINDOW_HEIGHT;
    float scale  = std::min(scaleX, scaleY);

    float scaledFontSize = _fontSize * scale;

    Vector2 textSize = MeasureTextEx(_font.getFont(), _text.c_str(), scaledFontSize, _spacing);
    Vector2 position = {
        box.x + (box.width - textSize.x) * 0.5f,
        box.y + (box.height - textSize.y) * 0.5f
    };
    
     DrawTextEx(_font.getFont(), _text.c_str(), position, scaledFontSize, _spacing, _color);
}

}