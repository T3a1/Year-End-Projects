/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** TextFont
*/

#include "TextFont.hpp"

namespace Raylib {

    TextFont::TextFont(std::string path, int size)
    {
        _font = LoadFontEx(path.c_str(), size, nullptr, 0);

        if (_font.texture.id == 0)
            throw Zappy::LoadingError("Failed to load font: " + path);
        SetTextureFilter(_font.texture, TEXTURE_FILTER_BILINEAR);
    }

    TextFont::~TextFont()
    {
        UnloadFont(_font);
    }

    Font& TextFont::getFont()
    {
        return _font;
    }
}
