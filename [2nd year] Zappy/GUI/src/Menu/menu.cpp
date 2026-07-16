/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** menu
*/

#include "menu.hpp"

namespace Zappy {

    Zappy::Menu::Menu(std::string port, std::string machine)
        : _port(std::move(port)), _machine(std::move(machine)),
        _menuMusic(MENU_MUSIC_PATH), _font(DEFAULT_FONT_PATH, 128),
        _rat{ 16, 16, 0, 0, Raylib::ImageTexture(MENU_RAT_TEXTURE_PATH) },
        _renderer()
    {
        _subwindowManager.addNormalizedSubWindow("menu",
            0.5f, 0.5f, 1.f, 1.f,
            0.5f, 0.5f, 1.f, 1.f, 4.f, 0
        );

        _subwindowManager.get("menu").setVisible(true);
        _subwindowManager.get("menu").recomputeRects();

        registerActions();

        if (_port != "X")
            _lockedFields = true;
        else
            _lockedFields = false;

        initImages();
        initButtons();
        initRats();

        _rat.frameWidth = _rat.texture.getTexture().width / _rat.columns;
        _rat.frameHeight = _rat.texture.getTexture().height / _rat.rows;

        _menuMusic.play(true);

    }

    Menu::~Menu() {

    }

    Rectangle Menu::getCenterPosition(float cx, float cy, float w, float h) const
    {
        float screenWidth = static_cast<float>(_subwindowManager.get("menu").getRect().width);
        float screenHeight = static_cast<float>(_subwindowManager.get("menu").getRect().height);

        float pixelW = w * screenWidth;
        float pixelH = h * screenHeight;

        float pixelX = cx * screenWidth - pixelW * 0.5f;
        float pixelY = cy * screenHeight - pixelH * 0.5f;

        return { pixelX, pixelY, pixelW, pixelH };
    }

    void Menu::initImages() {

        _images.emplace("Menu Background", Raylib::RectangleImage{
            WHITE,
            getCenterPosition(0.5f, 0.5f, 1.f, 1.f),
            MENU_BACKGROUND_TEXTURE_PATH
        });

        _images.emplace("Port Lock", Raylib::RectangleImage{
            WHITE,
            getCenterPosition(0.05f, 0.18f, 0.02f, 0.025f),
            MENU_LOCK_TEXTURE_PATH,
        });

        _images.emplace("Machine Lock", Raylib::RectangleImage{
            WHITE,
            getCenterPosition(0.05f, 0.33f, 0.02f, 0.025f),
            MENU_LOCK_TEXTURE_PATH        
        });

        _images.emplace("Main Panel", Raylib::RectangleImage{
            Color{0, 0, 0, 180},
            getCenterPosition(0.1f, 0.5f, 0.2f, 1.f), 
        });

        _images.emplace("Port Outline", Raylib::RectangleImage{
            GRAY,
            getCenterPosition(0.1f, 0.2f, 0.13f, 0.08f),
        });

        _images.emplace("Port Background", Raylib::RectangleImage{
            Color{0, 0, 0, 180},
            getCenterPosition(0.1f, 0.2f, 0.12f, 0.07f),
        });

        _images.emplace("Machine Outline", Raylib::RectangleImage{
            GRAY,
            getCenterPosition(0.1f, 0.35f, 0.13f, 0.08f),
        });

        _images.emplace("Machine Background", Raylib::RectangleImage{
             Color{0, 0, 0, 180},
            getCenterPosition(0.1f, 0.35f, 0.12f, 0.07f),     
        });

        _images.emplace("Select Field Outline", Raylib::RectangleImage{
            Color{255, 255, 255, 0},
            getCenterPosition(0.1f, 0.2f, 0.13f, 0.08f),
  
        });
    }

    void Menu::initButtons() {
        SubWindow &menu = _subwindowManager.get("menu");

        auto startButton = std::make_unique<Button>(
            MENU_BUTTON_TEXTURE_PATH, menu,
            Rectangle{ 0.1f, 0.55f, 0.12f, 0.1f }, Rectangle{ 0.f, 0.f, 0.f, 0.f },
            1.f, 1.f, "startGame", _actionMap, 0
        );
        _buttons.emplace("start", std::ref(*startButton));
        menu.addComponent(std::move(startButton));

        auto quitButton = std::make_unique<Button>(
            MENU_BUTTON_TEXTURE_PATH, menu,
            Rectangle{ 0.1f, 0.7f, 0.12f, 0.1f }, Rectangle{ 0.f, 0.f, 0.f, 0.f },
            1.f, 1.f, "quitGame", _actionMap, 0
        );
        _buttons.emplace("quit", std::ref(*quitButton));
        menu.addComponent(std::move(quitButton));

        auto portButton = std::make_unique<Button>(
          "", menu,
            Rectangle{ 0.1f, 0.2f, 0.12f, 0.07f }, Rectangle{ 0.f, 0.f, 0.f, 0.f },
            1.f, 1.f, "editPort", _actionMap, 0
        );
        _buttons.emplace("port", std::ref(*portButton));
        menu.addComponent(std::move(portButton));

        auto machineButton = std::make_unique<Button>(
            "", menu,
            Rectangle{ 0.1f, 0.35f, 0.12f, 0.07f }, Rectangle{ 0.f, 0.f, 0.f, 0.f },
            1.f, 1.f, "editMachine", _actionMap, 0
        );
        _buttons.emplace("machine", std::ref(*machineButton));
        menu.addComponent(std::move(machineButton));
    }

    void Menu::initRats() {
        _ratPositions.push_back(Zappy::Rat::ratPosition{ 0.4f, { 0.85f, 0.8f }, 1.f });
        _ratPositions.push_back(Zappy::Rat::ratPosition{ 0.3f, { 0.38f, 0.65f }, 1.f });
        _ratPositions.push_back(Zappy::Rat::ratPosition{ 0.1f, { 0.23f, 0.118f }, 1.f });
        _ratPositions.push_back(Zappy::Rat::ratPosition{ 0.5f, { 0.45f, 0.73f }, 1.f });
        _ratPositions.push_back(Zappy::Rat::ratPosition{ 0.15f, { 0.55f, 0.58f }, 1.f });
        _ratPositions.push_back(Zappy::Rat::ratPosition{ 0.3f, { 0.24f, 0.83f }, 0.3f });
        _ratPositions.push_back(Zappy::Rat::ratPosition{ 0.2f, { 0.56f, 0.332f }, 1.f });
        _ratPositions.push_back(Zappy::Rat::ratPosition{ 0.3f, { 0.6f, 0.95f }, 0.5f });
        _ratPositions.push_back(Zappy::Rat::ratPosition{ 0.15f, { 0.86f, 0.318f }, 1.f });
        _ratPositions.push_back(Zappy::Rat::ratPosition{ 0.2f, { 0.88f, 0.58f }, 0.7f });
        _ratPositions.push_back(Zappy::Rat::ratPosition{ 0.15f, { 0.37f, 0.435f }, 0.7f });
        _ratPositions.push_back(Zappy::Rat::ratPosition{ 0.2f, { 0.28f, 0.33f }, 1.f });
        _ratPositions.push_back(Zappy::Rat::ratPosition{ 0.2f, { 0.665f, 0.64f }, 1.f });
    }

    void Menu::registerActions() {
        _actionMap.registerAction(
            "startGame", [this]() {

                if (_port == "X" || _machine == "X") {
                    _correctValues = false;
                    return;
                }
                if (!tryToConnect()) {
                    _correctValues = false;
                    _lockedFields = false;
                    return;
                }
                _inMenu = false;
            }
        );
        _actionMap.registerAction(
            "quitGame", [this]() {
                _inMenu = false;
                _quitMenu = true;
            }
        );
        _actionMap.registerAction(
            "editPort", [this]() {
                if (_lockedFields)
                    return;
                _currentEditingField = "port";
                _images.at("Select Field Outline").setColor(Color{255, 255, 255, 150});
                _images.at("Select Field Outline").setRectangle(getCenterPosition(0.1f, 0.2f, 0.13f, 0.08f));
            }
        );
        _actionMap.registerAction(
            "editMachine", [this]() {
                if (_lockedFields)
                    return;
                _currentEditingField = "machine";
                _images.at("Select Field Outline").setColor(Color{255, 255, 255, 150});
                _images.at("Select Field Outline").setRectangle(getCenterPosition(0.1f, 0.35f, 0.13f, 0.08f));
            }
        );
    }

    void Menu::display() {
        while (!WindowShouldClose() && _inMenu) {
            if (IsKeyPressed(KEY_ESCAPE)) {
                _inMenu = false;
                _quitMenu = true;
            }

            float deltaTime = GetFrameTime();

            if (IsWindowResized()) {
                _subwindowManager.recomputeAllSubWindows();
                recomputeImages();
            }

            if (!_lockedFields)
                editTextField();

            _menuMusic.updateStream();

            processUI(deltaTime);
            BeginDrawing();
            ClearBackground(BLACK);
            {
                _renderer.drawMenuImages(_images, _lockedFields);
                _subwindowManager.get("menu").drawComponents();
                _renderer.drawMenuTexts(_subwindowManager.get("menu"), _font, _port, _machine, _correctValues);
                _renderer.drawMenuRats(_rat, _ratPositions, _subwindowManager.get("menu"));
            }
            EndDrawing();
        }

        if (_inMenu)
            _quitMenu = true;
    }

    void Menu::recomputeImages() {
        _images.at("Menu Background").setRectangle(getCenterPosition(0.5f, 0.5f, 1.f, 1.f));
        _images.at("Port Lock").setRectangle(getCenterPosition(0.05f, 0.18f, 0.02f, 0.025f));
        _images.at("Machine Lock").setRectangle(getCenterPosition(0.05f, 0.33f, 0.02f, 0.025f));
        _images.at("Main Panel").setRectangle(getCenterPosition(0.1f, 0.5f, 0.2f, 1.f));
        _images.at("Port Outline").setRectangle(getCenterPosition(0.1f, 0.2f, 0.13f, 0.08f));
        _images.at("Port Background").setRectangle(getCenterPosition(0.1f, 0.2f, 0.12f, 0.07f));
        _images.at("Machine Outline").setRectangle(getCenterPosition(0.1f, 0.35f, 0.13f, 0.08f));
        _images.at("Machine Background").setRectangle(getCenterPosition(0.1f, 0.35f, 0.12f, 0.07f));

        if (_currentEditingField == "port")
            _images.at("Select Field Outline").setRectangle(getCenterPosition(0.1f, 0.2f, 0.13f, 0.08f));
        else if (_currentEditingField == "machine")
            _images.at("Select Field Outline").setRectangle(getCenterPosition(0.1f, 0.35f, 0.13f, 0.08f));
    }

    void Menu::processUI(float deltaTime) {
        for (auto &subWindow : _subwindowManager.getAllActiveWindows()) {
            subWindow.get().update(deltaTime);
            subWindow.get().updateComponents(deltaTime);
        }

        Vector2 mousePos = GetMousePosition();
        bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        std::string topWindow = _subwindowManager.getFirstWindowInLayerOrder(mousePos);

        if (!topWindow.empty())
            _subwindowManager.get(topWindow).processMouseInput(mousePos, mousePressed);
        else {
            SubWindow &menu = _subwindowManager.get("menu");
            if (CheckCollisionPointRec(mousePos, menu.getRect()))
                menu.processMouseInput(mousePos, mousePressed);
        }
    }

    void Menu::editTextField() {
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (_currentEditingField == "port" && !_port.empty()) {
                _port.pop_back();
                if (_port.empty())
                    _port.push_back('X');
            }
            else if (_currentEditingField == "machine" && !_machine.empty()) {
                _machine.pop_back();
                if (_machine.empty())
                    _machine.push_back('X');
            }
        }

        int key = GetCharPressed();
        while (key > 0) {
            if (_currentEditingField == "port" && _port.size() < 5) {
                if (key >= '0' && key <= '9') {
                    if (_port == "X")
                        _port.clear();

                    _port += static_cast<char>(key);
                }
            }
            else if (_currentEditingField == "machine" && _machine.size() < 15) {
                if ((key >= '0' && key <= '9') || key == '.') {
                    if (_machine == "X")
                        _machine.clear();

                    _machine += static_cast<char>(key);
                }
            }

            key = GetCharPressed();
        }
    }

    bool Menu::tryToConnect() {
        try {
            Client client(_machine, std::stoi(_port));
            return client.connect();
        } catch (const std::exception &e) {
            return false;
        }
    }
}