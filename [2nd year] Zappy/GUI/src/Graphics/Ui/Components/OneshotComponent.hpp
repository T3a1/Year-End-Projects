/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** OneShotComponent
*/

#pragma once
#include "TextureComponent.hpp"
#include "ActionMap.hpp"
#include <string>


namespace Zappy {
    class OneshotComponent : public TextureComponent {
        public:
            OneshotComponent(const std::string &textureName, int frameCount,
                float fps, const std::string &triggerAction, ActionMap &actionMap,
                float x = 0.5f, float y = 0.5f, float width = 1.f, float height = 1.f,
                int layer = 0, int frameWidth = 0, int frameHeight = 0);

            void update(float deltaTime) override;
            void draw(const Rectangle &parentRect) const override;
            bool isDone() const { return _done; }

        private:
            void trigger();

            float _fps;
            float _elapsed;
            bool _playing;
            bool _done;
    };
}
