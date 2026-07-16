/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Camera
*/

#include "ViewCamera.hpp"
#include "Constants.hpp"
#include <cmath>
#include <algorithm>
 
namespace Raylib {

ViewCamera::ViewCamera(Vector3 startPos, float horizontalAngle, float verticalAngle, float fieldOfView)
    : _horizontalAngle(horizontalAngle), _verticalAngle(verticalAngle), _initialPos(startPos),
        _minX(0.f), _maxX(0.f), _minZ(0.f), _maxZ(0.f), _minY(0.5f)
{
    _camera.position = startPos;
    _camera.up = { 0.f, 1.f, 0.f };
    _camera.fovy = fieldOfView;
    _camera.projection = CAMERA_PERSPECTIVE;
    updateCameraTarget();
}

void ViewCamera::updateCameraTarget() {
    _camera.target = {
        _camera.position.x + cosf(_verticalAngle) * sinf(_horizontalAngle),
        _camera.position.y + sinf(_verticalAngle),
        _camera.position.z + cosf(_verticalAngle) * cosf(_horizontalAngle)
    };
}

void ViewCamera::clampPosition() {
    _camera.position.x = std::clamp(_camera.position.x, _minX, _maxX);
    _camera.position.z = std::clamp(_camera.position.z, _minZ, _maxZ);
    _camera.position.y = std::max(_camera.position.y, _minY);
}

void ViewCamera::setRectangularBounds(float minX, float maxX, float minZ, float maxZ, float minY) {
    _minX = minX;
    _maxX = maxX;
    _minZ = minZ;
    _maxZ = maxZ;
    _minY = minY;
}

void ViewCamera::moveForward(float delta) {
    _camera.position.x += cosf(_verticalAngle) * sinf(_horizontalAngle) * delta;
    _camera.position.z += cosf(_verticalAngle) * cosf(_horizontalAngle) * delta;

    clampPosition();
    updateCameraTarget();
}

void ViewCamera::moveRight(float delta) {
    _camera.position.x += cosf(_horizontalAngle) * delta;
    _camera.position.z += -sinf(_horizontalAngle) * delta;

    clampPosition();
    updateCameraTarget();
}

void ViewCamera::moveUp(float delta) {
    _camera.position.y += delta;

    clampPosition();
    updateCameraTarget();
}

void ViewCamera::rotateLook(float horizontalRotation, float verticalRotation) {
    _horizontalAngle += horizontalRotation;
    _verticalAngle += verticalRotation;

    if (_verticalAngle >  MAX_VERTICAL_ANGLE)
        _verticalAngle =  MAX_VERTICAL_ANGLE;
    if (_verticalAngle < -MAX_VERTICAL_ANGLE)
        _verticalAngle = -MAX_VERTICAL_ANGLE;

    updateCameraTarget();
}

void ViewCamera::setMapInfo(Vector3 mapCenter, float heightFactor) {
    float mapDiagonal = sqrtf(powf(_maxX - mapCenter.x, 2) + powf(_maxZ - mapCenter.z, 2));
    const float factor = 0.75f;

    _halfPosition.x = mapCenter.x + factor * (_maxX - mapCenter.x);
    _halfPosition.z = mapCenter.z + factor * (_maxZ - mapCenter.z);
    _halfPosition.y = factor * mapDiagonal * heightFactor * 0.5f;

    float dx = mapCenter.x - _halfPosition.x;
    float dz = mapCenter.z - _halfPosition.z;
    float distance = sqrtf(dx*dx + dz*dz);
    float dy = mapCenter.y - _halfPosition.y;

    float angleToCenter = atan2f(dy, distance);
    _halfVerticalAngle = angleToCenter;
    _halfHorizontalAngle = -PI * 0.75f;
}

void ViewCamera::resetToHalfway() {
    _camera.position = _halfPosition;
    _horizontalAngle = _halfHorizontalAngle;
    _verticalAngle = _halfVerticalAngle;

    updateCameraTarget();
}

}
