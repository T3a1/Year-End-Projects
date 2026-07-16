/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** Food
*/

#include "Food.hpp"

namespace Zappy {
    Food::Food(std::string modelPath, Vector3 position,
        Vector3 rotation, float scale, ModelCache& modelCache)
        : AEntity(modelPath, position, rotation, scale, modelCache),
        _bobPhase(static_cast<float>(rand() % 1000) / 1000.0f * 2.0f * PI)
    {
    }

    Food::~Food()
    {
}
}

