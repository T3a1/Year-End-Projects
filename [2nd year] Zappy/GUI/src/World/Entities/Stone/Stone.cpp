/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** Stone
*/

#include "Stone.hpp"

namespace Zappy {
    Stone::Stone(std::string modelPath, Vector3 position, Vector3 rotation,
        float scale, StoneType type, ModelCache& modelCache)
        : AEntity(modelPath, position, rotation, scale, modelCache), _type(type),
        _bobPhase(static_cast<float>(rand() % 1000) / 1000.0f * 2.0f * PI)
    {
    }

    Stone::~Stone()
    {
    }
}
