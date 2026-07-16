/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** ModelCache
*/

#ifndef MODELCACHE_HPP_
#define MODELCACHE_HPP_

#include <string>
#include <map>
#include "raylib.h"

namespace Zappy {
    class ModelCache {

        public:
            ModelCache();
            ~ModelCache();
            
            const Model& getModel(const std::string& path);
            void clearCache();

        private:
            std::map<std::string, Model> _cache;
    };
}

#endif /* !MODELCACHE_HPP_ */
