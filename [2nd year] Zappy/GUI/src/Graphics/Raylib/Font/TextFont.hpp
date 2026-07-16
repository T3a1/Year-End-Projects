/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** Text Font
*/

#ifndef TEXT_FONT_HPP_
#define TEXT_FONT_HPP_

#include "raylib.h"
#include <string>
#include <map>
#include "Errors.hpp"

namespace Raylib {
    class TextFont {
        public:
            TextFont(std::string path, int size);
            ~TextFont();

            Font& getFont();

        protected:
        private:
            Font _font;
    };
}

#endif /* !TEXT_FONT_HPP_ */
