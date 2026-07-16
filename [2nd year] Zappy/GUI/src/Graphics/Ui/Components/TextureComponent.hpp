/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** TextureComponent
*/

#pragma once
#include "ASubWindowComponent.hpp"
#include <string>
#include "ImageTexture.hpp"

namespace Zappy {
    class TextureComponent : public ASubWindowComponent {
        public:
            TextureComponent( const std::string &texturePath,
                float x = 0.5f, float y = 0.5f, float width = 1.f, float height = 1.f,
                int layer = 0, int frameCount = 1, int frameWidth = 0, int frameHeight = 0);
            ~TextureComponent() override;

            void update([[maybe_unused]]float deltaTime) override {};
            void draw(const Rectangle &parentRect) const override;

        protected:
            int _frameCount;
            int _frameWidth;
            int _frameHeight;
            int _currentFrame;
            Raylib::ImageTexture _texture;
    };
}
