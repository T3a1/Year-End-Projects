/*
** EPITECH PROJECT, 2026
** GUI
** File description:
** AnimationHandler
*/

#include "AnimationHandler.hpp"

namespace Zappy {
    AnimationHandler::AnimationHandler()
    {
    }

    AnimationHandler::~AnimationHandler()
    {
        for (auto& anim : _animationCache)
            UnloadModelAnimations(anim.second.anims, anim.second.frameCount);
        _animationCache.clear();
    }

    void AnimationHandler::playAnimation(const std::string& name,
        ModelAnimation*& anims, int& animCount,
        bool& isLoaded, int& currentAnim,
        int& animFrame, float& animTime)
    {
        auto animIt = _animationCache.find(name);

        if (animIt == _animationCache.end()) {
            std::cerr << "Animation not found: " << name << std::endl;
            return;
        }
        
        anims = animIt->second.anims;
        animCount = animIt->second.frameCount;
        isLoaded = true;
        currentAnim = 0;
        animFrame = 0;
        animTime = 0.f;
    }

    void AnimationHandler::cacheAnimations()
    {
        for (const auto& pair : _animationMapping) {
            int count = 0;
            ModelAnimation* anims = LoadModelAnimations(pair.second.c_str(), &count);
            if (!anims || count <= 0)
                throw Zappy::LoadingError("Failed to load animation: " + pair.second);
            if (anims && count > 0)
                _animationCache[pair.first] = { anims, count };
        }
    }

    void AnimationHandler::changeTexture(Model& model, const std::string& textureName)
    {
        auto texture = _TextureCache.find(textureName);
        
        if (texture == _TextureCache.end()) {
            std::cerr << "Texture not found: " << textureName << std::endl;
            return;
        }

        for (int i = 0; i < model.materialCount; i++) {
            model.materials[i].maps[MATERIAL_MAP_ALBEDO].texture = texture->second.getTexture();
        }
    }

}
