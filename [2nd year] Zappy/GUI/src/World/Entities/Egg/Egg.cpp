/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** Egg
*/

#include "Egg.hpp"

namespace Zappy {
    Egg::Egg(int id, std::string team, Vector3 position, AnimationHandler& animationHandler, int timeUnit, ModelCache& modelCache)
        : AEntity(EGG_MODEL_PATH, position, Vector3{-90.f, 0.f, 0.f}, 0.01f, modelCache, true), _id(id),
            _team(team), _animationHandler(animationHandler), _timeUnit(timeUnit)
    {
        _animationHandler.playAnimation("Stand Up",
            _anims, _animCount, isLoaded(), _currentAnim, _animFrame, _animTime);
        manageAnimations(0.f);

        _animationHandler.changeTexture(getModel(), "Level 1");
    }

    Egg::~Egg()
    {
    }

    void Egg::manageAnimations(float deltaTime)
    {
        if (_animCount <= 0 || _anims == nullptr)
            return;

        float speed = PLAYER_BASE_ANIMATION_SPEED;
        _animTime += deltaTime * speed;

        _animFrame = static_cast<int>(_animTime) % _anims[_currentAnim].frameCount;

        UpdateModelAnimation(getModel(), _anims[_currentAnim], _animFrame);
    }

}
