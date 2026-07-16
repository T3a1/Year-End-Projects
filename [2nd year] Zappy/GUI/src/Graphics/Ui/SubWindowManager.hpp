/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** SubWindowManager
*/

#pragma once
#include "SubWindow.hpp"
#include <vector>
#include <memory>
#include <string>
#include <functional>

namespace Zappy {
    struct SubWin {
        std::string name;
        std::unique_ptr<SubWindow> window;
    };

    class SubWindowManager {
        public:
            SubWindowManager();
            ~SubWindowManager() = default;

            SubWindow &addNormalizedSubWindow(const std::string &name,
                float x1, float y1, float w1, float h1,
                float x2, float y2, float w2, float h2,
                float animationSpeed, int layerIndex);
            void closeAllSubWindows(const std::string &exception,
                const std::function<void(const std::string &name)> &onClose);
            void clear();
            void recomputeAllSubWindows();
            
            SubWindow &get(const std::string &name);
            const SubWindow &get(const std::string& name) const;
            std::vector<std::reference_wrapper<SubWindow>> getAllActiveWindows();
            std::string getFirstWindowInLayerOrder(const Vector2 &mousePos) const;

        private:
            std::vector<SubWin> _subwindows;
    };
}
