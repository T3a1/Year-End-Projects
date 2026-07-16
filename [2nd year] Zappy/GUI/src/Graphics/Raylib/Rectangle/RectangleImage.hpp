/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** RectangleImage
*/

#ifndef RECTANGLEIMAGE_HPP_
#define RECTANGLEIMAGE_HPP_

#include "raylib.h"
#include "ImageTexture.hpp"

namespace Raylib {
    class RectangleImage {
        public:
            RectangleImage( Color color, Rectangle rectangle, std::string texturePath = "");
            RectangleImage(const RectangleImage&) = delete;
            RectangleImage& operator=(const RectangleImage&) = delete;
            ~RectangleImage();

            RectangleImage(RectangleImage&& other) noexcept
                : _texture(std::move(other._texture)),
                  _color(other._color),
                  _rectangle(other._rectangle)
            {}

            RectangleImage& operator=(RectangleImage&& other) noexcept {
                if (this != &other) {
                    _texture   = std::move(other._texture);
                    _color     = other._color;
                    _rectangle = other._rectangle;
                }
                return *this;
            }

            

            const ImageTexture& getImageTexture() const { return _texture; }
            const Color& getColor() const { return _color; }
            const Rectangle& getRectangle() const { return _rectangle; }

            void setColor(const Color& color) { _color = color; }
            void setRectangle(const Rectangle& rectangle) { _rectangle = rectangle; }
            void setTexture(const std::string& texturePath) { _texture.changeTexture(texturePath); }

        protected:
        private:
            ImageTexture _texture;
            Color _color;
            Rectangle _rectangle;
            
    };
}

#endif /* !RECTANGLEIMAGE_HPP_ */
