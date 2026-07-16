/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** BookHandler
*/

#include "BookHandler.hpp"
#include "SubWindow.hpp"
#include "Button.hpp"
#include "OneshotComponent.hpp"
#include "GameInfoComponent.hpp"
#include "Constants.hpp"
#include "Core.hpp"

namespace Zappy {

BookHandler::BookHandler(ActionMap &actionMap, MusicManager &musicManager, Core& core)
    : _pageContainer(nullptr), _actionMap(actionMap), _musicManager(musicManager),
    _settingsButtons(), _font(DEFAULT_FONT_PATH, 120), _core(core)
{

    registerBookActions();
}


void BookHandler::registerBookActions() {










    _actionMap.registerAction("Volume 1", [this]() {
        _settingsData.volume = 0.0f;
        _settingsButtons["Volume 1"]->setTexture(SETTINGS_FILLED_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Volume 2"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Volume 3"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Volume 4"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Volume 5"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _musicManager.updateVolume(0.0f);
    });

    _actionMap.registerAction("Volume 2", [this]() {
        _settingsData.volume = 0.25f;
        _settingsButtons["Volume 1"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Volume 2"]->setTexture(SETTINGS_FILLED_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Volume 3"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Volume 4"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Volume 5"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _musicManager.updateVolume(0.25f);
    });

    _actionMap.registerAction("Volume 3", [this]() {
        _settingsData.volume = 0.5f;
        _settingsButtons["Volume 1"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Volume 2"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Volume 3"]->setTexture(SETTINGS_FILLED_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Volume 4"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Volume 5"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _musicManager.updateVolume(0.5f);
    });

    _actionMap.registerAction("Volume 4", [this]() {
        _settingsData.volume = 0.75f;
        _settingsButtons["Volume 1"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Volume 2"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Volume 3"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Volume 4"]->setTexture(SETTINGS_FILLED_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Volume 5"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _musicManager.updateVolume(0.75f);
    });

    _actionMap.registerAction("Volume 5", [this]() {
        _settingsData.volume = 1.0f;
        _settingsButtons["Volume 1"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Volume 2"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Volume 3"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Volume 4"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Volume 5"]->setTexture(SETTINGS_FILLED_TICKBOX_TEXTURE_PATH);
        _musicManager.updateVolume(1.0f);
    });

    _actionMap.registerAction("Camera Speed 1", [this]() {
        _settingsData.cameraSpeed = 0.1f;
        _settingsButtons["Camera Speed 1"]->setTexture(SETTINGS_FILLED_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Camera Speed 2"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Camera Speed 3"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Camera Speed 4"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Camera Speed 5"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
    });

    _actionMap.registerAction("Camera Speed 2", [this]() {
        _settingsData.cameraSpeed = 0.3f;
        _settingsButtons["Camera Speed 1"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Camera Speed 2"]->setTexture(SETTINGS_FILLED_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Camera Speed 3"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Camera Speed 4"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Camera Speed 5"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
    });

    _actionMap.registerAction("Camera Speed 3", [this]() {
        _settingsData.cameraSpeed = 0.5f;
        _settingsButtons["Camera Speed 1"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Camera Speed 2"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Camera Speed 3"]->setTexture(SETTINGS_FILLED_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Camera Speed 4"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Camera Speed 5"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
    });

    _actionMap.registerAction("Camera Speed 4", [this]() {
        _settingsData.cameraSpeed = 0.7f;
        _settingsButtons["Camera Speed 1"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Camera Speed 2"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Camera Speed 3"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Camera Speed 4"]->setTexture(SETTINGS_FILLED_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Camera Speed 5"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
    });

    _actionMap.registerAction("Camera Speed 5", [this]() {
        _settingsData.cameraSpeed = 0.9f;
        _settingsButtons["Camera Speed 1"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Camera Speed 2"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Camera Speed 3"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Camera Speed 4"]->setTexture(SETTINGS_TICKBOX_TEXTURE_PATH);
        _settingsButtons["Camera Speed 5"]->setTexture(SETTINGS_FILLED_TICKBOX_TEXTURE_PATH);
    });

    _actionMap.registerAction("Fullscreen", [this]() {

        if ( _settingsData.windowMode == FULLSCREEN)
            return;
        _settingsData.windowMode = FULLSCREEN;
        ToggleFullscreen();
    });

    _actionMap.registerAction("Windowed", [this]() {
        if ( _settingsData.windowMode == WINDOWED)
            return;
        _settingsData.windowMode = WINDOWED;
        ToggleFullscreen();
    });










}

void BookHandler::buildAboutPage(ContainerComponent &container, [[maybe_unused]] SubWindow &infoWindow) {

    container.addChild(std::make_unique<TextComponent>(
        "About", _font, 80.f, BLACK,
        0.5f, 0.1f, 0.5f, 0.5f, 4
    ));

    container.addChild(std::make_unique<TextComponent>(
        "Zappy is a network multiplayer game on a\n"
        "tiled map where multiple teams compete to\n"
        "collect resources and level up. The winning\n"
        "team is the first one to have at least 6\n"
        "players reach the maximum level of 8 through\n"
        "an incantation ritual. The world contains\n"
        "food and six stone types. Each player must\n"
        "gather food to survive and stones for elevation.\n"
        "\n"
        "Players can move, turn, look around, check\n"
        "inventory, broadcast, take/drop objects, fork\n"
        "to create new slots, and eject others from the\n"
        "same tile. Commands have execution times based\n"
        "on the time unit (f).\n"
        "\n"
        "To level up, a player initiates an incantation\n"
        "while a required number of same-level players\n"
        "and specific stones are on the same tile. The\n"
        "ritual freezes participants; success removes\n"
        "the stones and raises everyone involved. The\n"
        "requirements vary on which level to attain. The\n"
        "first team with 6 players at level 8 wins the game.",
        _font, 20.f, BLACK,
        0.26f, 0.46f, 0.8f, 0.6f, 4
    ));

    auto getInfo = [this]() -> GameInfo {
        return _core.getGameInfo();
    };
    container.addChild(std::make_unique<GameInfoComponent>(
        getInfo, _font, 0.75f, 0.55f, 0.5f, 1.f, 4
    ));
}




void BookHandler::buildSettingsPage(ContainerComponent &container, SubWindow &infoWindow) {


    container.addChild(std::make_unique<TextComponent>(
        "Settings", _font, 80.f, BLACK,
        0.5f, 0.1f, 0.5f, 0.5f, 4
    ));

    container.addChild(std::make_unique<TextComponent>(
        "Volume", _font, 50.f, BLACK,
        0.25f, 0.2f, 0.5f, 0.5f, 4
    ));

    container.addChild(std::make_unique<TextComponent>(
        "1", _font, 20.f, BLACK,
        0.08f, 0.3f, 0.5f, 0.5f, 4
    ));

    container.addChild(std::make_unique<TextComponent>(
        "2", _font, 20.f, BLACK,
        0.16f, 0.3f, 0.5f, 0.5f, 4
    ));

    container.addChild(std::make_unique<TextComponent>(
        "3", _font, 20.f, BLACK,
        0.24f, 0.3f, 0.5f, 0.5f, 4
    ));

    container.addChild(std::make_unique<TextComponent>(
        "4", _font, 20.f, BLACK,
        0.32f, 0.3f, 0.5f, 0.5f, 4
    ));

    container.addChild(std::make_unique<TextComponent>(
        "5", _font, 20.f, BLACK,
        0.40f, 0.3f, 0.5f, 0.5f, 4
    ));

    container.addChild(std::make_unique<TextComponent>(
        "Camera Speed", _font, 50.f, BLACK,
        0.25f, 0.55f, 0.5f, 0.5f, 4
    ));

    container.addChild(std::make_unique<TextComponent>(
        "1", _font, 20.f, BLACK,
        0.08f, 0.65f, 0.5f, 0.5f, 4
    ));

    container.addChild(std::make_unique<TextComponent>(
        "2", _font, 20.f, BLACK,
        0.16f, 0.65f, 0.5f, 0.5f, 4
    ));

    container.addChild(std::make_unique<TextComponent>(
        "3", _font, 20.f, BLACK,
        0.24f, 0.65f, 0.5f, 0.5f, 4
    ));

    container.addChild(std::make_unique<TextComponent>(
        "4", _font, 20.f, BLACK,
        0.32f, 0.65f, 0.5f, 0.5f, 4
    ));

    container.addChild(std::make_unique<TextComponent>(
        "5", _font, 20.f, BLACK,
        0.40f, 0.65f, 0.5f, 0.5f, 4
    ));

    


    auto volumeButton1 = std::make_shared<Button>(
        SETTINGS_TICKBOX_TEXTURE_PATH, infoWindow,
        Rectangle{0.08f, 0.35f, 0.05f, 0.05f}, Rectangle{0.f, 0.f, 0.f, 0.f},
        0.f, 0.f, "Volume 1", _actionMap, 4
    );

    auto volumeButton2 = std::make_shared<Button>(
        SETTINGS_TICKBOX_TEXTURE_PATH, infoWindow,
        Rectangle{0.16f, 0.35f, 0.05f, 0.05f}, Rectangle{0.f, 0.f, 0.f, 0.f},
        0.f, 0.f, "Volume 2", _actionMap, 4
    );

    auto volumeButton3 = std::make_shared<Button>(
        SETTINGS_FILLED_TICKBOX_TEXTURE_PATH, infoWindow,
        Rectangle{0.24f, 0.35f, 0.05f, 0.05f}, Rectangle{0.f, 0.f, 0.f, 0.f},
        0.f, 0.f, "Volume 3", _actionMap, 4
    );

    auto volumeButton4 = std::make_shared<Button>(
        SETTINGS_TICKBOX_TEXTURE_PATH, infoWindow,
        Rectangle{0.32f, 0.35f, 0.05f, 0.05f}, Rectangle{0.f, 0.f, 0.f, 0.f},
        0.f, 0.f, "Volume 4", _actionMap, 4
    );

    auto volumeButton5 = std::make_shared<Button>(
        SETTINGS_TICKBOX_TEXTURE_PATH, infoWindow,
        Rectangle{0.40f, 0.35f, 0.05f, 0.05f}, Rectangle{0.f, 0.f, 0.f, 0.f},
        0.f, 0.f, "Volume 5", _actionMap, 4
    );

    _settingsButtons["Volume 1"] = volumeButton1;
    _settingsButtons["Volume 2"] = volumeButton2;
    _settingsButtons["Volume 3"] = volumeButton3;
    _settingsButtons["Volume 4"] = volumeButton4;
    _settingsButtons["Volume 5"] = volumeButton5;

    container.addSharedChild(volumeButton1);
    container.addSharedChild(volumeButton2);
    container.addSharedChild(volumeButton3);
    container.addSharedChild(volumeButton4);
    container.addSharedChild(volumeButton5);
    

    auto cameraSpeedButton1 = std::make_shared<Button>(
        SETTINGS_TICKBOX_TEXTURE_PATH, infoWindow,
        Rectangle{0.08f, 0.7f, 0.05f, 0.05f}, Rectangle{0.f, 0.f, 0.f, 0.f},
        0.f, 0.f, "Camera Speed 1", _actionMap, 4
    );

    auto cameraSpeedButton2 = std::make_shared<Button>(
        SETTINGS_FILLED_TICKBOX_TEXTURE_PATH, infoWindow,
        Rectangle{0.16f, 0.7f, 0.05f, 0.05f}, Rectangle{0.f, 0.f, 0.f, 0.f},
        0.f, 0.f, "Camera Speed 2", _actionMap, 4
    );

    auto cameraSpeedButton3 = std::make_shared<Button>(
        SETTINGS_TICKBOX_TEXTURE_PATH, infoWindow,
        Rectangle{0.24f, 0.7f, 0.05f, 0.05f}, Rectangle{0.f, 0.f, 0.f, 0.f},
        0.f, 0.f, "Camera Speed 3", _actionMap, 4
    );

    auto cameraSpeedButton4 = std::make_shared<Button>(
        SETTINGS_TICKBOX_TEXTURE_PATH, infoWindow,
        Rectangle{0.32f, 0.7f, 0.05f, 0.05f}, Rectangle{0.f, 0.f, 0.f, 0.f},
        0.f, 0.f, "Camera Speed 4", _actionMap, 4
    );

    auto cameraSpeedButton5 = std::make_shared<Button>(
        SETTINGS_TICKBOX_TEXTURE_PATH, infoWindow,
        Rectangle{0.40f, 0.7f, 0.05f, 0.05f}, Rectangle{0.f, 0.f, 0.f, 0.f},
        0.f, 0.f, "Camera Speed 5", _actionMap, 4
    );

    _settingsButtons["Camera Speed 1"] = cameraSpeedButton1;
    _settingsButtons["Camera Speed 2"] = cameraSpeedButton2;
    _settingsButtons["Camera Speed 3"] = cameraSpeedButton3;
    _settingsButtons["Camera Speed 4"] = cameraSpeedButton4;
    _settingsButtons["Camera Speed 5"] = cameraSpeedButton5;

    container.addSharedChild(cameraSpeedButton1);
    container.addSharedChild(cameraSpeedButton2);
    container.addSharedChild(cameraSpeedButton3);
    container.addSharedChild(cameraSpeedButton4);
    container.addSharedChild(cameraSpeedButton5);



    auto fullscreenButton = std::make_shared<Button>(
        SETTINGS_WINDOW_BUTTON_TEXTURE_PATH, infoWindow,
        Rectangle{0.65f, 0.7f, 0.2f, 0.2f}, Rectangle{0.f, 0.f, 0.f, 0.f},
        0.f, 0.f, "Fullscreen", _actionMap, 4
    );

    
    auto windowButton = std::make_shared<Button>(
        SETTINGS_WINDOW_BUTTON_TEXTURE_PATH, infoWindow,
        Rectangle{0.85f, 0.7f, 0.2f, 0.2f}, Rectangle{0.f, 0.f, 0.f, 0.f},
        0.f, 0.f, "Windowed", _actionMap, 4
    );
    
    
    container.addSharedChild(windowButton);
    container.addSharedChild(fullscreenButton);

    container.addSharedChild(std::make_unique<TextComponent>(
        " FULL\nSCREEN", _font, 25.f, BLACK,
        0.65f, 0.7f, 0.5f, 0.5f, 5
    ));
    
    container.addSharedChild(std::make_unique<TextComponent>(
        "WINDOWED", _font, 20.f, BLACK,
        0.85f, 0.7f, 0.5f, 0.5f, 5
    ));

    _frequencyText = std::make_shared<TextComponent>(
        std::to_string(_settingsData.frequency), _font, 60.f, BLACK,
        0.75f, 0.3f, 0.3f, 0.3f, 4
    );
    container.addSharedChild(_frequencyText);

    auto minusButton = std::make_shared<Button>(
        MINUS_ICON_TEXTURE_PATH, infoWindow,
        Rectangle{0.62f, 0.3f, 0.08f, 0.08f}, Rectangle{0.f, 0.f, 0.f, 0.f},
        0.f, 0.f, "Minus Frequency", _actionMap, 4
    );

    auto plusButton = std::make_shared<Button>(
        PLUS_ICON_TEXTURE_PATH, infoWindow,
        Rectangle{0.88f, 0.3f, 0.08f, 0.08f}, Rectangle{0.f, 0.f, 0.f, 0.f},
        0.f, 0.f, "Plus Frequency", _actionMap, 4
    );

    container.addSharedChild(minusButton);
    container.addSharedChild(plusButton);

    container.addSharedChild(std::make_unique<TextComponent>(
        "Frequency", _font, 50.f, BLACK,
        0.75f, 0.2f, 0.5f, 0.5f, 4
    ));
}











void BookHandler::buildInputsPage(ContainerComponent &container, [[maybe_unused]] SubWindow &infoWindow) {
    container.addChild(std::make_unique<TextComponent>(
        "Inputs", _font, 80.f, BLACK,
        0.5f, 0.1f, 0.5f, 0.5f, 4
    ));

    container.addChild(std::make_unique<TextComponent>(
        "Infos :", _font, 25.f, BLACK,
        0.1f, 0.2f, 0.2f, 0.2f, 4
    ));

    container.addChild(std::make_unique<TextComponent>(
        "Players infos :", _font, 25.f, BLACK,
        0.145f, 0.3f, 0.2f, 0.2f, 4
    ));

    container.addChild(std::make_unique<TextComponent>(
        "Broadcast chat :", _font, 25.f, BLACK,
        0.15f, 0.4f, 0.2f, 0.2f, 4
    ));

    container.addChild(std::make_unique<TextComponent>(
        "Manage Grid :", _font, 25.f, BLACK,
        0.14f, 0.5f, 0.2f, 0.2f, 4
    ));

    container.addChild(std::make_unique<TextComponent>(
        "Manage Tiles :", _font, 25.f, BLACK,
        0.14f, 0.6f, 0.2f, 0.2f, 4
    ));

    container.addChild(std::make_unique<TextComponent>(
        "Select Tile:", _font, 25.f, BLACK,
        0.12f, 0.7f, 0.2f, 0.2f, 4
    ));

    container.addChild(std::make_unique<TextComponent>(
        "Navigate Tiles:", _font, 25.f, BLACK,
        0.14f, 0.8f, 0.2f, 0.2f, 4
    ));

    container.addChild(std::make_unique<TextComponent>(
        "Select Player:", _font, 25.f, BLACK,
        0.65f, 0.2f, 0.2f, 0.2f, 4
    ));

    container.addChild(std::make_unique<TextComponent>(
        "Switch Camera", _font, 25.f, BLACK,
        0.65f, 0.3f, 0.2f, 0.2f, 4
    ));

    container.addChild(std::make_unique<TextComponent>(
        "Reset Camera :", _font, 25.f, BLACK,
        0.66f, 0.4f, 0.2f, 0.2f, 4
    ));

    container.addChild(std::make_unique<TextComponent>(
        "Move Up :", _font, 25.f, BLACK,
        0.64f, 0.5f, 0.2f, 0.2f, 4
    ));

    container.addChild(std::make_unique<TextComponent>(
        "Move Down :", _font, 25.f, BLACK,
        0.65f, 0.6f, 0.2f, 0.2f, 4
    ));



    container.addChild(std::make_unique<TextComponent>(
        "Drone :", _font, 25.f, BLACK,
        0.63f, 0.7f, 0.2f, 0.2f, 4
    ));





    container.addChild(std::make_unique<TextureComponent>(
        SETTINGS_KEY_BUTTON, 0.4f, 0.2f, 0.08f, 0.08f, 4
    ));

    container.addChild(std::make_unique<TextureComponent>(
        SETTINGS_KEY_BUTTON, 0.4f, 0.3f, 0.08f, 0.08f, 4
    ));

    container.addChild(std::make_unique<TextureComponent>(
        SETTINGS_KEY_BUTTON, 0.4f, 0.4f, 0.08f, 0.08f, 4
    ));

    container.addChild(std::make_unique<TextureComponent>(
        SETTINGS_KEY_BUTTON, 0.4f, 0.5f, 0.08f, 0.08f, 4
    ));

    container.addChild(std::make_unique<TextureComponent>(
        SETTINGS_KEY_BUTTON, 0.4f, 0.6f, 0.08f, 0.08f, 4
    ));

    container.addChild(std::make_unique<TextureComponent>(
        SETTINGS_KEY_BUTTON, 0.4f, 0.7f, 0.08f, 0.08f, 4
    ));

    container.addChild(std::make_unique<TextureComponent>(
        SETTINGS_KEY_BUTTON, 0.4f, 0.8f, 0.08f, 0.08f, 4
    ));

    container.addChild(std::make_unique<TextureComponent>(
        SETTINGS_KEY_BUTTON, 0.9f, 0.2f, 0.08f, 0.08f, 4
    ));

    container.addChild(std::make_unique<TextureComponent>(
        SETTINGS_KEY_BUTTON, 0.9f, 0.3f, 0.08f, 0.08f, 4
    ));

    container.addChild(std::make_unique<TextureComponent>(
        SETTINGS_KEY_BUTTON, 0.9f, 0.4f, 0.08f, 0.08f, 4
    ));

    container.addChild(std::make_unique<TextureComponent>(
        SETTINGS_KEY_BUTTON, 0.9f, 0.5f, 0.08f, 0.08f, 4
    ));

    container.addChild(std::make_unique<TextureComponent>(
        SETTINGS_KEY_BUTTON, 0.9f, 0.6f, 0.08f, 0.08f, 4
    ));
    
    container.addChild(std::make_unique<TextureComponent>(
        SETTINGS_KEY_BUTTON, 0.7f, 0.8f, 0.08f, 0.08f, 4
    ));
    
    container.addChild(std::make_unique<TextureComponent>(
        SETTINGS_KEY_BUTTON, 0.8f, 0.8f, 0.08f, 0.08f, 4
    ));
    
    
    container.addChild(std::make_unique<TextureComponent>(
        SETTINGS_KEY_BUTTON, 0.9f, 0.8f, 0.08f, 0.08f, 4
    ));
    
    container.addChild(std::make_unique<TextureComponent>(
        SETTINGS_KEY_BUTTON, 0.8f, 0.7f, 0.08f, 0.08f, 4
    ));



    container.addChild(std::make_unique<TextComponent>(
        "I", _font, 20.f, BLACK,
        0.4f, 0.2f, 0.5f, 0.5f, 5
    ));

    container.addChild(std::make_unique<TextComponent>(
        "P", _font, 20.f, BLACK,
        0.4f, 0.3f, 0.5f, 0.5f, 5
    ));

    container.addChild(std::make_unique<TextComponent>(
        "C", _font, 20.f, BLACK,
        0.4f, 0.4f, 0.5f, 0.5f, 5
    ));

    container.addChild(std::make_unique<TextComponent>(
        "G", _font, 20.f, BLACK,
        0.4f, 0.5f, 0.5f, 0.5f, 5
    ));

    container.addChild(std::make_unique<TextComponent>(
        "T", _font, 20.f, BLACK,
        0.4f, 0.6f, 0.5f, 0.5f, 5
    ));

    container.addChild(std::make_unique<TextComponent>(
        "Click", _font, 18.f, BLACK,
        0.4f, 0.7f, 0.5f, 0.5f, 5
    ));

    container.addChild(std::make_unique<TextComponent>(
        "<-->", _font, 20.f, BLACK,
        0.4f, 0.8f, 0.5f, 0.5f, 5
    ));

    container.addChild(std::make_unique<TextComponent>(
        "Click", _font, 18.f, BLACK,
        0.9f, 0.2f, 0.5f, 0.5f, 5
    ));

    container.addChild(std::make_unique<TextComponent>(
        "TAB", _font, 18.f, BLACK,
        0.9f, 0.3f, 0.5f, 0.5f, 5
    ));

    container.addChild(std::make_unique<TextComponent>(
        "R", _font, 20.f, BLACK,
        0.9f, 0.4f, 0.5f, 0.5f, 5
    ));

    container.addChild(std::make_unique<TextComponent>(
        "Space", _font, 15.f, BLACK,
        0.9f, 0.5f, 0.5f, 0.5f, 5
    ));

    container.addChild(std::make_unique<TextComponent>(
        "Shift", _font, 15.f, BLACK,
        0.9f, 0.6f, 0.5f, 0.5f, 5
    ));


    container.addChild(std::make_unique<TextComponent>(
        "Q", _font, 20.f, BLACK,
        0.7f, 0.8f, 0.5f, 0.5f, 5
    ));

    container.addChild(std::make_unique<TextComponent>(
        "S", _font, 20.f, BLACK,
        0.8f, 0.8f, 0.5f, 0.5f, 5
    ));

    container.addChild(std::make_unique<TextComponent>(
        "D", _font, 20.f, BLACK,
        0.9f, 0.8f, 0.5f, 0.5f, 5
    ));

    container.addChild(std::make_unique<TextComponent>(
        "Z", _font, 20.f, BLACK,
        0.8f, 0.7f, 0.5f, 0.5f, 5
    ));




}










void BookHandler::buildCreditsPage(ContainerComponent &container, [[maybe_unused]] SubWindow &infoWindow) {
    container.addChild(std::make_unique<TextComponent>(
        "Levels", _font, 80.f, BLACK,
        0.25f, 0.1f, 0.5f, 0.5f, 4
    ));

    container.addChild(std::make_unique<TextComponent>(
        "Credits", _font, 80.f, BLACK,
        0.75f, 0.1f, 0.5f, 0.5f, 4
    ));

    std::vector<std::string> skinPaths = {
        LEVEL_1_SKIN_PATH, LEVEL_2_SKIN_PATH, LEVEL_3_SKIN_PATH, LEVEL_4_SKIN_PATH,
        LEVEL_5_SKIN_PATH, LEVEL_6_SKIN_PATH, LEVEL_7_SKIN_PATH, LEVEL_8_SKIN_PATH
    };

    const float textureW = 0.09f;
    const float textureH = 0.28f;
    const float startX = 0.04f;
    const float colStep = 0.115f;
    const float row0Y = 0.3f;
    const float row1Y = 0.63f;

    for (int i = 0; i < 8; i++) {
        int col = i % 4;
        float x = startX + col * colStep + textureW * 0.5f;
        float y = (i < 4) ? row0Y : row1Y;

        container.addChild(std::make_unique<TextureComponent>(
            skinPaths[i], x, y, textureW, textureH, 4
        ));

        container.addChild(std::make_unique<TextComponent>(
            "Level " + std::to_string(i + 1), _font, 18.f, BLACK,
            x, y + textureH * 0.55f, textureW, 0.08f, 5
        ));
    }

    const std::vector<std::pair<std::string, std::string>> memberInfo = {
        { "Leandre Fouret", "Server" },
        { "David Caron", "Server" },
        { "Arthur Piron", "IA" },
        { "Eliott Duchene", "GUI" },
        { "Arthur Vignes", "GUI" },
    };

    const float rightStartX = 0.75f;
    const float rightStartY = 0.25f;
    const float memberStep  = 0.12f;

    for (size_t i = 0; i < memberInfo.size(); i++) {
        float y = rightStartY + i * memberStep;

        container.addChild(std::make_unique<TextComponent>(
            memberInfo[i].first, _font, 28.f, BLACK,
            rightStartX, y, 0.5f, 0.5f, 4
        ));
        container.addChild(std::make_unique<TextComponent>(
            "(" + memberInfo[i].second + ")", _font, 20.f, DARKGRAY,
            rightStartX, y + 0.03f, 0.5f, 0.5f, 4
        ));
    }
}







void BookHandler::init(SubWindow &infoWindow) {
    auto container = std::make_shared<ContainerComponent>(0.5f, 0.5f, 0.702f, 0.851f, 3);
    _pageContainer = container;
    infoWindow.addComponent(container);

    _pageBuilders["about"] = [this](ContainerComponent &c, SubWindow &w) { buildAboutPage(c, w); };
    _pageBuilders["settings"] = [this](ContainerComponent &c, SubWindow &w) { buildSettingsPage(c, w); };
    _pageBuilders["inputs"] = [this](ContainerComponent &c, SubWindow &w) { buildInputsPage(c, w); };
    _pageBuilders["credits"] = [this](ContainerComponent &c, SubWindow &w) { buildCreditsPage(c, w); };

    _currentPage = "";
}










void BookHandler::switchPage(const std::string &pageName, SubWindow &infoWindow) {
    if (_currentPage == pageName)
        return;

    _currentPage = pageName;
    _pageContainer->clearChildren();

    auto it = _pageBuilders.find(pageName);
    if (it != _pageBuilders.end())
        it->second(*_pageContainer, infoWindow);
}

void BookHandler::setFrequency(int frequency) {
    _settingsData.frequency = frequency;
    if (_frequencyText)
        _frequencyText->setText(std::to_string(frequency));
}


}
