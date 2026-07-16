/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** Rat
*/

#ifndef RAT_HPP_
#define RAT_HPP_

#include "ImageTexture.hpp"

namespace Zappy {

    class Rat {

        public:

            struct ratInfo {
                int columns = 16;
                int rows = 16;
                int frameWidth = 0;
                int frameHeight = 0;
                Raylib::ImageTexture texture;
            };

            struct ratPosition {
                float scale;
                Vector2 position;
                float brightness;
            };

            Rat() = default;
            ~Rat() = default;

        protected:
        private:
    };
}

#endif /* !RAT_HPP_ */
