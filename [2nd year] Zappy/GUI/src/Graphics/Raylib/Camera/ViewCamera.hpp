/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Camera
*/

#pragma once
#include "raylib.h"

namespace Raylib {
    class ViewCamera {
        public:
            ViewCamera() = default;
            ViewCamera(Vector3 startPos, float horizontalAngle, float verticalAngle, float fieldOfView);
            ~ViewCamera() = default;
            ViewCamera(const ViewCamera &) = delete;

            void moveForward(float delta);
            void moveRight(float delta);
            void moveUp(float delta);
            void rotateLook(float horizontalRotation, float verticalRotation);
            void resetView();
            void setRectangularBounds(float minX, float maxX, float minZ, float maxZ, float minY);
            void setMapInfo(Vector3 mapCenter, float heightFactor = 0.4f);
            void resetToHalfway();

            Camera3D &getCamera() { return _camera; }
            const Camera3D &getCamera() const { return _camera; }

        private:
            void updateCameraTarget();
            void clampPosition();

            Camera3D _camera;
            float _horizontalAngle;
            float _verticalAngle;
            Vector3 _initialPos;

            float _minX;
            float _maxX;
            float _minZ;
            float _maxZ;
            float _minY;

            Vector3 _halfPosition;
            float _halfHorizontalAngle;
            float _halfVerticalAngle;
    };
}
