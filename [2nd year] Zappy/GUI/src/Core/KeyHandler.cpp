/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** KeyHandler
*/

#include "KeyHandler.hpp"
#include "Constants.hpp"

namespace Zappy {

KeyHandler::KeyHandler(ActionMap &actionMap, std::function<bool()> panelOpenCheck)
    : _actionMap(actionMap), _cameraMode(CameraMode::Drone),
    _panelOpenCheck(panelOpenCheck)
{
    DisableCursor();
}

void KeyHandler::update(Raylib::ViewCamera &camera, float droneSpeed)
{
    handleGenericInputs();

    if (_panelOpenCheck())
        return;

    const std::function<void()> modeActions[] = {
        [&]() { handleDroneCameraInput(camera, droneSpeed); },
        [&]() { handleFixedCameraInput(); },
    };
    modeActions[static_cast<int>(_cameraMode)]();
}

void KeyHandler::handleGenericInputs()
{
    static const std::vector<std::pair<int, std::string>> KEY_ACTIONS = {
        {KEY_ESCAPE, "closeProgram"},
        {KEY_C, "toggleChat"},
        {KEY_I, "toggleInfo"},
        {KEY_G, "toggleGrid"},
        {KEY_P, "togglePlayers"},
        {KEY_X, "toggleMarkers"},
        {KEY_T, "toggleTile"},
        {KEY_ENTER, "selectNextTile"},
        {KEY_UP, "moveTileUp"},
        {KEY_DOWN, "moveTileDown"},
        {KEY_LEFT, "moveTileLeft"},
        {KEY_RIGHT, "moveTileRight"}
    };

    for (const auto& [key, action] : KEY_ACTIONS)
        if (IsKeyPressed(key))
            _actionMap.trigger(action);

    if (_panelOpenCheck())
        return;

    if (IsKeyPressed(KEY_TAB)) {
        const std::function<void()> nextMode[] = {
            [&]() { _cameraMode = CameraMode::Fixed; EnableCursor(); },
            [&]() { _cameraMode = CameraMode::Drone; DisableCursor(); },
        };
        nextMode[static_cast<int>(_cameraMode)]();
    }
}

void KeyHandler::handleDroneCameraInput(Raylib::ViewCamera &camera, float droneSpeed) {
    float speed = droneSpeed * GetFrameTime() * 60.f;

    if (IsKeyDown(KEY_R))
        _actionMap.trigger("resetCamera");

    const std::vector<std::pair<int, std::function<void()>>> keyActions = {
        { KEY_W, [&]{ camera.moveForward(speed); } },
        { KEY_S, [&]{ camera.moveForward(-speed); } },
        { KEY_A, [&]{ camera.moveRight(speed); } }, 
        { KEY_D, [&]{ camera.moveRight(-speed); } },
        { KEY_SPACE, [&]{ camera.moveUp(speed); } },
        { KEY_LEFT_SHIFT, [&]{ camera.moveUp(-speed); } },
    };

    for (const auto &[key, action] : keyActions)
        if (IsKeyDown(key))
            action();

    Vector2 mouseDelta = GetMouseDelta();
    if (mouseDelta.x != 0.f || mouseDelta.y != 0.f)
        camera.rotateLook(
           -mouseDelta.x * DRONE_LOOK_SPEED,
           -mouseDelta.y * DRONE_LOOK_SPEED
        );
}

void KeyHandler::handleFixedCameraInput() {
    if (IsKeyDown(KEY_R))
        _actionMap.trigger("resetCamera");
}

}
