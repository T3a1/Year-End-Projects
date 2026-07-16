/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** ImageTexture
*/

#ifndef IMAGETEXTURE_HPP_
#define IMAGETEXTURE_HPP_
#include "raylib.h"
#include <string>
#include <map>
#include "Errors.hpp"

namespace Raylib {
    class ImageTexture {
        public:
            ImageTexture(const std::string& filePath);
            ImageTexture(Texture texture) : _texture(texture) {}
            ImageTexture(ImageTexture&& other) noexcept : _texture(other._texture) { other._texture = {};}
            ImageTexture() : _texture({}) {}
            ImageTexture(const ImageTexture&) = delete;
            ImageTexture& operator=(const ImageTexture&) = delete;

            ~ImageTexture();

            const Texture& getTexture() const { return _texture; }
            Texture& getTexture() { return _texture; }
            void changeTexture(const std::string& filePath);


            ImageTexture& operator=(ImageTexture&& other) noexcept {
                if (this != &other) {
                    UnloadTexture(_texture);
                    _texture = other._texture;
                    other._texture = {};
                }
                return *this;
            }


        protected:
        private:

        Texture _texture;
    };
}

#endif /* !IMAGETEXTURE_HPP_ */