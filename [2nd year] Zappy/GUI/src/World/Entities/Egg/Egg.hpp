/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** Egg
*/

#ifndef EGG_HPP_
#define EGG_HPP_

#include "AEntity.hpp"
#include "Constants.hpp"
#include "AnimationHandler.hpp"
#include "ModelCache.hpp"
#include "ImageTexture.hpp"

namespace Zappy {
    class Egg : public AEntity {
        public:
            Egg(int id, std::string team, Vector3 position, AnimationHandler& animationHandler, int timeUnit, ModelCache& modelCache);
            ~Egg();

            void manageAnimations(float deltaTime);
            int getId() const { return _id; }
            std::string getTeam() const { return _team; }
            void setTimeUnit(int timeUnit) { _timeUnit = timeUnit; }

        protected:

        private:

            int _id;
            std::string _team;
            AnimationHandler& _animationHandler;
            int _animCount = 0;
            ModelAnimation* _anims = nullptr;
            int _animFrame = 0;
            int _currentAnim = 0;
            float _animTime = 0.f;

            int _timeUnit = 0;
            
    };
}

#endif /* !EGG_HPP_ */
