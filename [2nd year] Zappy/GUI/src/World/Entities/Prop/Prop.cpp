/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** Prop
*/

#include "Prop.hpp"

namespace Zappy {
    Prop::Prop(std::string modelPath, Vector3 position, Vector3 rotation, float scale, ModelCache& modelCache)
        : AEntity(modelPath, position, rotation, scale, modelCache)
    {
    }

    Prop::~Prop()
    {
    }
}
