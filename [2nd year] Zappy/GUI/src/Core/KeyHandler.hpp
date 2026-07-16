/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** KeyHandler
*/

#pragma once
#include "SubWindowManager.hpp"
#include "ActionMap.hpp"
#include <vector>
#include <functional>
#include "ViewCamera.hpp"

namespace Zappy {
    class Camera;
    enum class CameraMode { Drone, Fixed };

    class KeyHandler {
        public:
            KeyHandler(ActionMap &actionMap, std::function<bool()> panelOpenCheck);
            ~KeyHandler() = default;
            KeyHandler(const KeyHandler &keyHandler) = delete;
            KeyHandler &operator=(const KeyHandler &keyHandler) = delete;

            void update(Raylib::ViewCamera &camera, float droneSpeed);
            bool isDroneCameraMode() const { return _cameraMode == CameraMode::Drone; }
            void setCameraMode(CameraMode mode) { _cameraMode = mode;}

        private:
            void handleGenericInputs();
            void handleDroneCameraInput(Raylib::ViewCamera &camera, float droneSpeed);
            void handleFixedCameraInput();

            ActionMap &_actionMap;
            CameraMode _cameraMode;
            std::function<bool()> _panelOpenCheck;
            
    };
}
