/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** ModelCache
*/

#include "ModelCache.hpp"

namespace Zappy {

    ModelCache::ModelCache()
    {
    }

    ModelCache::~ModelCache()
    {
        clearCache();
    }

    const Model& ModelCache::getModel(const std::string& path)
    {
        auto it = _cache.find(path);
        if (it != _cache.end())
            return it->second;

        Model loaded = LoadModel(path.c_str());
        if (loaded.meshCount == 0 || loaded.meshes == nullptr)
            loaded = LoadModelFromMesh(GenMeshPlane(1, 1, 1, 1));

        _cache[path] = loaded;
        return _cache[path];
    }

    void ModelCache::clearCache()
    {
        for (auto& [path, model] : _cache)
            UnloadModel(model);
        _cache.clear();
    }
}

