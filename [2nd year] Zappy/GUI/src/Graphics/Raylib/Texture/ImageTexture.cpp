/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** ImageTexture
*/

#include "ImageTexture.hpp"

namespace Raylib {
    ImageTexture::ImageTexture(const std::string& filePath) : _texture({})
    {
        if (filePath.empty())
            return;

        _texture = LoadTexture(filePath.c_str());
        
        if (_texture.id == 0)
            throw Zappy::LoadingError("Failed to load texture: " + filePath);
    }

    ImageTexture::~ImageTexture()
    {
        if (_texture.id != 0)
            UnloadTexture(_texture);
    }

    void ImageTexture::changeTexture(const std::string& filePath)
    {
        if (filePath.empty())
            return;
        UnloadTexture(_texture);
        
        _texture = LoadTexture(filePath.c_str());

        if (_texture.id == 0)
            throw Zappy::LoadingError("Failed to load texture: " + filePath);
    }
}
