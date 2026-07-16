/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** RectangleImage
*/

#include "RectangleImage.hpp"

namespace Raylib {
    RectangleImage::RectangleImage( Color color, Rectangle rectangle, std::string texturePath)
        : _texture(ImageTexture(texturePath)), _color(color), _rectangle(rectangle)
    {

    }

    RectangleImage::~RectangleImage()
    {
    }
}


