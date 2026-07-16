/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** Food
*/

#ifndef Food_HPP_
#define Food_HPP_

#include "AEntity.hpp"

namespace Zappy {
    class Food : public AEntity {
        public:
            Food(std::string modelPath, Vector3 position, Vector3 rotation,
                float scale, ModelCache& modelCache);
            ~Food();

            float getBobPhase() const { return _bobPhase; }

        protected:
        private:
            float _bobPhase;
    };
}

#endif /* !Food_HPP_ */
