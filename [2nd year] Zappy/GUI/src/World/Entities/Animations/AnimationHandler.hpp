/*
** EPITECH PROJECT, 2026
** GUI
** File description:
** AnimationHandler
*/

#ifndef ANIMATIONHANDLER_HPP_
#define ANIMATIONHANDLER_HPP_

#include <map>
#include <string>
#include "raylib.h"
#include "iostream"
#include "Constants.hpp"
#include "Errors.hpp"
#include "ImageTexture.hpp"
namespace Zappy {

    struct PreloadedAnimation {
        ModelAnimation* anims;
        int frameCount;
    };

    class AnimationHandler {
        public:
            AnimationHandler();
            ~AnimationHandler();

            void setAnimationMapping(std::map<std::string, std::string> animationMapping) { _animationMapping = animationMapping; }
            void playAnimation(const std::string& name, ModelAnimation*& anims, int& animCount, bool& isLoaded, int& currentAnim, int& animFrame, float& animTime);
            void cacheAnimations();

           void setTextureCache(std::map<std::string, Raylib::ImageTexture>&& textureCache) { _TextureCache = std::move(textureCache); }
            void changeTexture(Model& model, const std::string& textureName);

        protected:
        private:
            std::map<std::string, std::string> _animationMapping;

            std::map<std::string, PreloadedAnimation> _animationCache;

            std::map<std::string, Raylib::ImageTexture> _TextureCache;

    };
}

#endif /* !ANIMATIONHANDLER_HPP_ */
