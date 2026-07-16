/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** LoopingComponent
*/

#pragma once
#include "TextureComponent.hpp"

namespace Zappy {
    class LoopingComponent : public TextureComponent {
        public:
            LoopingComponent(const std::string &texturePath, int frameCount,
                float fps, float x = 0.5f, float y = 0.5f, float width = 1.f, float height = 1.f,
                int layer = 0, int frameWidth = 0, int frameHeight = 0);

            void update(float deltaTime) override;

        private:
            float _fps;
            float _elapsed;
    };
}
