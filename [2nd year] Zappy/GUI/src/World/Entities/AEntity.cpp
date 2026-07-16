/*
** EPITECH PROJECT, 2026
** GUI
** File description:
** AEntity
*/

#include "AEntity.hpp"
#include <cmath>

namespace Zappy {

AEntity::AEntity(std::string modelPath, Vector3 position, Vector3 rotation, float scale, ModelCache& modelCache, bool shared)
    : _isLoaded(false), _model{}, _position(position), _rotation(rotation), _scale(scale), _modelCache(modelCache)
    {
        if (shared)
            _model = _modelCache.getModel(modelPath);
        else {
            _model = LoadModel(modelPath.c_str());
            if (_model.meshCount == 0 || _model.meshes == nullptr)
                _model = LoadModelFromMesh(GenMeshCube(1, 1, 1));
        }
        _isLoaded = true;
        setRotation(_rotation);
    }

    AEntity::~AEntity()
    {
    }

    float AEntity::getDistance(const IEntity& other) const
    {
        float dx = (getPosition().x - other.getPosition().x) * (getPosition().x - other.getPosition().x);
        float dy = (getPosition().y - other.getPosition().y) * (getPosition().y - other.getPosition().y);
        float dz = (getPosition().z - other.getPosition().z) * (getPosition().z - other.getPosition().z);

        return std::sqrt(dx + dy + dz);
    }

    float AEntity::getDistance(const Vector3& point) const
    {
        float dx = (getPosition().x - point.x) * (getPosition().x - point.x);
        float dy = (getPosition().y - point.y) * (getPosition().y - point.y);
        float dz = (getPosition().z - point.z) * (getPosition().z - point.z);
        return std::sqrt(dx + dy + dz);
    }

}