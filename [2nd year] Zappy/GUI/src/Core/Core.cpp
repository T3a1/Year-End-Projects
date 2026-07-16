/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** Core
*/

#include "Core.hpp"
#include "Constants.hpp"
#include "TextureComponent.hpp"
#include "LoopingComponent.hpp"
#include "OneshotComponent.hpp"
#include "Errors.hpp"
#include <cmath>
#include <iostream>
#include "raylib.h"
#include <limits>
    
namespace Zappy {

Core::Core(float mapDensity)
    : _shouldClose(false), _camera(nullptr), _renderer(),
      _actionMap(), _keyHandler(_actionMap, [this]() { return isAnyPanelOpen(); }),
      _subwindowManager(), _tickAccumulator(0.f), _buttons(), _linksButtonWindow(),
      _playerAnimationHandler(), _bookHandler(std::make_unique<BookHandler>(_actionMap, _musicManager, *this)),
      _client(nullptr), _protocolHandler(nullptr), _timeUnit(0), _isMapReady(false),
      _modelCache(), _map(std::make_unique<Map>(1, 1, _modelCache)), _mapDensity(mapDensity),
      _waitingFont(DEFAULT_FONT_PATH, 120), _musicManager(), _pageFlipSound(PAGE_FLIP_SOUND_PATH)
{
    initSubWindows();
    registerActions();
    initPlayers();

}

Core::~Core() {
    _buttons.clear();
    _subwindowManager.clear();
    _bookHandler.reset();
    _protocolHandler.reset();
}

void Core::initSubWindows() {
    _subwindowManager.addNormalizedSubWindow("main",
        0.5f, 0.5f, 1.f, 1.f,
        0.5f, 0.5f, 1.f, 1.f, 1.f, 0
    );
    _subwindowManager.addNormalizedSubWindow("players",
        -0.15f, 0.5875f, 0.2f, 0.775f,
        0.115f, 0.5875f, 0.2f, 0.775f, 4.f, 1
    );
    _subwindowManager.addNormalizedSubWindow("chat",
        1.125f, 0.5f, 0.25f, 1.f,
        0.875f, 0.5f, 0.25f, 1.f, 4.f, 2
    );
    _subwindowManager.addNormalizedSubWindow("info",
        0.5f, -0.4f, 0.7f, 0.7f,
        0.5f,  0.5f, 0.7f, 0.7f, 4.f, 2
    );
    _subwindowManager.addNormalizedSubWindow("tile",
        1.f, 0.8f, 0.15f, 0.4f,
        0.91f, 0.785f, 0.15f, 0.4f, 4.f, 1
    );

    initSubWindowsContent();
    initButtons();
    switchPage("about", false);
}

void Core::initSubWindowsContent() {
    initMainWindow();
    initPlayersWindow();
    initChatWindow();
    initInfoWindow();
    initTileWindow();
}

void Core::initMainWindow() {   
    _subwindowManager.get("main").setVisible(true);
}

void Core::initPlayersWindow() {
    SubWindow &players = _subwindowManager.get("players");
    players.setVisible(false);

    auto getPlayer = [this]() -> const Player* {
        if (_players.empty() || _selectedPlayerIndex < 0
            || _selectedPlayerIndex >= static_cast<int>(_players.size()))
            return nullptr;
        
        auto it = _players.begin();
        std::advance(it, _selectedPlayerIndex);
        return &it->second;
    };

    _playerPanel = std::make_unique<PlayerPanelHandler>(_actionMap);
    _playerPanel->init(players, getPlayer);
}

void Core::initChatWindow() {
    SubWindow &chat = _subwindowManager.get("chat");
    chat.setVisible(false);

    auto onSelectPlayer = [this](int id) {
        int index = 0;
        for (const auto& [playerId, player] : _players) {
            if (playerId == id) {
                _selectedPlayerIndex = index;
                _actionMap.trigger("togglePlayers");
                return;
            }
            index++;
        }
    };

    auto getTeamColor = [this](const std::string& team) -> Color {
        return this->getTeamColor(team);
    };

    _chatPanel = std::make_unique<ChatPanelHandler>();
    _chatPanel->init(chat, onSelectPlayer, getTeamColor);
}

void Core::initInfoWindow() {
    SubWindow &info = _subwindowManager.get("info");

    info.setVisible(false);
    info.addComponent(std::make_shared<TextureComponent>(
       BOOK_COVER_TEXTURE_PATH, 0.5f, 0.5f, 1.f, 1.f, 0
    ));
    info.addComponent(std::make_shared<TextureComponent>(
        BOOK_PAGE_TEXTURE_PATH, 0.5f, 0.411f, 0.702f, 0.851f, 2
    ));
    info.addComponent(std::make_shared<OneshotComponent>(
        BOOK_PAGES_TEXTURE_PATH, 10, 16.f,
        "turnBookPages", _actionMap, 0.5f, 0.411f, 0.702f, 0.851f, 5
    ));

    _bookHandler->init(info);
}

void Core::initTileWindow() {
    SubWindow &tile = _subwindowManager.get("tile");
    tile.setVisible(false);

    auto getTile = [this]() -> const Tile* {
        if (_selectedTileCoords.x < 0 || _selectedTileCoords.y < 0)
            return nullptr;

        int x = static_cast<int>(_selectedTileCoords.x);
        int y = static_cast<int>(_selectedTileCoords.y);

        if (x >= _map->getWidth() || y >= _map->getHeight())
            return nullptr;
        return _map->getCurrentTile(x, y).get();
    };

    _tilePanel = std::make_unique<TilePanelHandler>(_actionMap);
    _tilePanel->init(tile, getTile);
}

void Core::initButtons() {
    initMainButtons();
    initInfoButtons();
}

void Core::initMainButtons() {
    SubWindow &main = _subwindowManager.get("main");

    auto closeButton = std::make_shared<Button>(
        CLOSE_BUTTON_TEXTURE_PATH, main,
        Rectangle{0.0525f, 0.0575f, 0.09f, 0.09f},
        Rectangle{0.0525f, 0.0575f, 0.09f, 0.09f},
        1.f, 1.f, "closeProgram", _actionMap, 1
    );
    _buttons["main"].emplace("close", std::ref(*closeButton));
    main.addComponent(closeButton);

    auto playersButton = std::make_shared<Button>(
        PLAYERS_BUTTON_TEXTURE_PATH, main,
        Rectangle{0.0525f, 0.1575f, 0.09f, 0.09f},
        Rectangle{0.0525f, 0.1575f, 0.09f, 0.09f},
        0.f, 4.f, "togglePlayers", _actionMap, 1
    );
    _buttons["main"].emplace("players", std::ref(*playersButton));
    main.addComponent(playersButton);

    auto infoButton = std::make_shared<Button>(
        INFO_BUTTON_TEXTURE_PATH, main,
        Rectangle{0.1625f, 0.0575f, 0.09f, 0.09f},
        Rectangle{0.1625f, 0.0575f, 0.09f, 0.09f},
        1.f, 1.f, "toggleInfo", _actionMap, 1
    );
    _buttons["main"].emplace("info", std::ref(*infoButton));
    main.addComponent(infoButton);

    auto chatButton = std::make_shared<Button>(
        CHAT_BUTTON_TEXTURE_PATH, main,
        Rectangle{0.9475f, 0.0575f, 0.09f, 0.09f},
        Rectangle{0.6975f, 0.0575f, 0.09f, 0.09f},
        0.f, 4.f, "toggleChat", _actionMap, 1
    );
    _buttons["main"].emplace("chat", std::ref(*chatButton));
    _linksButtonWindow.emplace("chat", _buttons["main"].at("chat"));
    main.addComponent(chatButton);

    auto gridButton = std::make_shared<Button>(
        GRID_BUTTON_TEXTURE_PATH, main,
        Rectangle{0.2725f, 0.0575f, 0.09f, 0.09f},
        Rectangle{0.2725f, 0.0575f, 0.09f, 0.09f},
        0.f, 4.f, "toggleGrid", _actionMap, 1
    );
    _buttons["main"].emplace("grid", std::ref(*gridButton));
    main.addComponent(gridButton);
}

void Core::initInfoButtons() {
    SubWindow &info = _subwindowManager.get("info");

    auto makeBookmark = [&](const std::string &name, float yPos) {
        std::string actionName = "switchPage" +
            std::string(1, std::toupper(name[0])) + name.substr(1);

        std::string textureName;

        switch (name[0]) {
            case 'a':
                textureName = ABOUT_BOOKMARK_TEXTURE_PATH;
                break;
            case 's':
                textureName = SETTINGS_BOOKMARK_TEXTURE_PATH;
                break;
            case 'i':
                textureName = INPUTS_BOOKMARK_TEXTURE_PATH;
                break;
            case 'c':
                textureName = CREDITS_BOOKMARK_TEXTURE_PATH;
                break;
            default:
                throw std::invalid_argument("Invalid bookmark name: " + name);
        }

        auto button = std::make_shared<Button>( 
            textureName, info,
            Rectangle{0.83f, yPos, 0.25f, 0.1f},
            Rectangle{0.88f, yPos, 0.25f, 0.1f},
            1.f, 4.f, actionName, _actionMap, 2
        );
        _buttons["info"].emplace(name, std::ref(*button));
        info.addComponent(button);
    };

    makeBookmark("about", 0.26f);
    makeBookmark("settings", 0.38f);
    makeBookmark("inputs", 0.5f);
    makeBookmark("credits", 0.62f);
}

void Core::registerActions() {
    auto synchronizeButton = [this](const std::string &name) {
        auto it = _linksButtonWindow.find(name);
        if (it != _linksButtonWindow.end())
            it->second.get().togglePosition();
    };

    _actionMap.registerAction("closeProgram", [this]() { _shouldClose = true; });

    _actionMap.registerAction("togglePlayers", [this, synchronizeButton]() {
        _subwindowManager.closeAllSubWindows("players", synchronizeButton);
        _subwindowManager.get("players").togglePosition();
        _showSelectedTeamOnly = !_showSelectedTeamOnly;
        updateMarkers();
        auto selectedPlayer = getSelectedPlayer();
        if (_subwindowManager.get("players").isVisible() && !_players.empty())
            _client->send("plv #" + std::to_string(selectedPlayer->get().getId()));
    });

   _actionMap.registerAction("toggleInfo", [this, synchronizeButton]() {
        _subwindowManager.closeAllSubWindows("info", synchronizeButton);
        bool infoVisible = _subwindowManager.get("info").togglePosition();

        if (infoVisible)
            EnableCursor();
        else {
            DisableCursor();
            _keyHandler.setCameraMode(CameraMode::Drone);
        }
    });

    _actionMap.registerAction("toggleChat", [this, synchronizeButton]() {
        _subwindowManager.closeAllSubWindows("chat", synchronizeButton);
        _subwindowManager.get("chat").togglePosition();
        _buttons["main"].at("chat").get().togglePosition();
    });

    _actionMap.registerAction("resetCamera", [this]() {
        if (_protocolHandler && _protocolHandler->isMapInitialized() && _camera)
            _camera->resetToHalfway();
    });

    std::vector<std::string> pages = {"about", "settings", "inputs", "credits"};
    for (const auto &page : pages) {
        std::string actionName = "switchPage";
        actionName += std::string(1, std::toupper(page[0])) + page.substr(1);
        _actionMap.registerAction(actionName, [this, page]() { switchPage(page, true); });
    }
    _actionMap.registerAction("toggleGrid", [this]() { _showGrid = !_showGrid; });

    _actionMap.registerAction("prevPlayer", [this]() {
        if (_players.empty())
            return;
        _selectedPlayerIndex = (_selectedPlayerIndex - 1 + static_cast<int>(_players.size())) % static_cast<int>(_players.size());
        auto selectedPlayer = getSelectedPlayer();
        _client->send("plv #" + std::to_string(selectedPlayer->get().getId()));
        updateMarkers();
    });

    _actionMap.registerAction("nextPlayer", [this]() {
        if (_players.empty())
            return;
        _selectedPlayerIndex = (_selectedPlayerIndex + 1) % static_cast<int>(_players.size());
        auto selectedPlayer = getSelectedPlayer();
        _client->send("plv #" + std::to_string(selectedPlayer->get().getId()));
        updateMarkers();
    });

    _actionMap.registerAction("toggleMarkers", [this]() {
        _markersEnabled = !_markersEnabled;
        updateMarkers();
    });

    _actionMap.registerAction("toggleTile", [this, synchronizeButton]() {
        _subwindowManager.closeAllSubWindows("tile", synchronizeButton);
        _subwindowManager.get("tile").togglePosition();
    });

    _actionMap.registerAction("selectNextTile", [this]() {
        if (!_map || _selectedTileCoords.x == -1 || _selectedTileCoords.y == -1)
            return;

        int width = _map->getWidth();
        int height = _map->getHeight();
        int x = static_cast<int>(_selectedTileCoords.x);
        int y = static_cast<int>(_selectedTileCoords.y);
        int nextX = (x + 1) % width;
        int nextY = (nextX == 0) ? (y + 1) % height : y;
        _selectedTileCoords = { static_cast<float>(nextX), static_cast<float>(nextY) };
    });

    _actionMap.registerAction("moveTileUp", [this]() { moveTile(0, -1); });
    _actionMap.registerAction("moveTileDown", [this]() { moveTile(0, 1); });
    _actionMap.registerAction("moveTileLeft", [this]() { moveTile(-1, 0); });
    _actionMap.registerAction("moveTileRight", [this]() { moveTile(1, 0); });

    _actionMap.registerAction("Minus Frequency", [this]() {

        int newUnit = std::max(10, _timeUnit - 10);
        _client->send("sst " + std::to_string(newUnit));
    });

    _actionMap.registerAction("Plus Frequency", [this]() {
        int newUnit = std::min(100, _timeUnit + 10);
        _client->send("sst " + std::to_string(newUnit));
    });
}

void Core::switchPage(const std::string &pageName, bool playSound) {
    if (_bookHandler->getCurrentPage() == pageName)
        return;

    if (playSound)
        _pageFlipSound.play(false);
    if (_buttons["info"].count(_bookHandler->getCurrentPage()))
        _buttons["info"].at(_bookHandler->getCurrentPage()).get().togglePosition();
    if (_buttons["info"].count(pageName))
        _buttons["info"].at(pageName).get().togglePosition();

    _bookHandler->switchPage(pageName, _subwindowManager.get("info"));
    _actionMap.trigger("turnBookPages");
}

void Core::initConnection(const std::string &host, int port) {

    _client = std::make_unique<Client>(host, port);
    _protocolHandler = std::make_unique<ProtocolHandler>(*_client, *this);

    if (!_client->connect())
        throw NetworkError("NetworkError: Failed to connect to server at "
            + host + ":" + std::to_string(port));

    _client->send("GRAPHIC");
}

bool Core::isAnyPanelOpen() const {
    const std::vector<std::string> panels = {"info"};

    for (const auto& name : panels)
        if (_subwindowManager.get(name).isVisible())
            return true;

    return false;
}

Color Core::getTeamColor(const std::string& teamName) {
    auto it = _teamColors.find(teamName);
    if (it != _teamColors.end())
        return it->second;

    Color color = {
        static_cast<unsigned char>(rand() % 256),
        static_cast<unsigned char>(rand() % 256),
        static_cast<unsigned char>(rand() % 256),
        255
    };

    _teamColors[teamName] = color;
    return color;
}

GameInfo Core::getGameInfo() const {
    GameInfo info;
    info.mapWidth = _map->getWidth();
    info.mapHeight = _map->getHeight();
    info.teamNames = _teamNames;
    info.teamCount = static_cast<int>(_teamNames.size());
    info.totalResources.fill(0);
    info.deadPlayers = _numberDeadPlayers;
    info.playersPerLevel.fill(0);

    for (const auto& [id, player] : _players) {
        const std::string& team = player.getTeam();
        info.playerCountPerTeam[team]++;

        int level = player.getLevel();
        if (level >= 1 && level <= 8) {
            info.playersPerLevel[level]++;
            info.playersPerLevelPerTeam[team][level]++;
        }

        const auto& inv = player.getInventory();
        for (int i = 0; i < RESOURCE_COUNT; ++i)
            info.totalResources[i] += inv[i];
    }

    for (int y = 0; y < _map->getHeight(); y++) {
        for (int x = 0; x < _map->getWidth(); x++) {
            const auto& tile = _map->getCurrentTile(x, y);
            info.totalResources[0] += tile->getFoodCount();

            auto stoneCounts = tile->getStoneCounts();
            for (int i = 0; i < 6; i++)
                info.totalResources[i + 1] += stoneCounts[static_cast<StoneType>(i)];
        }
    }
    return info;
}

void Core::checkSetupCamera() {
    if (!_protocolHandler || !_protocolHandler->isMapInitialized() || _camera)
        return;

    int width = _map->getWidth();
    Vector3 mapCenter = {
        (width - 1) * MAP_REF_TILE_SIZE * 0.5f, 0.f,
        (_map->getHeight() - 1) * MAP_REF_TILE_SIZE * 0.5f
    };
    float bound = width * MAP_REF_TILE_SIZE * CAM_BOUND_FACTOR;

    _camera = std::make_unique<Raylib::ViewCamera>(
        Vector3{ mapCenter.x, scaleToScreen(CAM_HEIGHT),
                 mapCenter.z - scaleToScreen(CAM_Z_OFFSET) },
        0.5f, 0.f, CAM_FOV
    );
    _camera->setRectangularBounds(-bound, bound, -bound, bound, DRONE_MIN_Y);
    _camera->setMapInfo(mapCenter, 0.3f);
    _camera->resetToHalfway();
}

Vector2 Core::getTileUnderMouse(const Vector2& mousePosition) const {
    if (!_camera || !_map || !_map->getWidth() || !_map->getHeight())
        return {-1, -1};

    Ray ray = GetMouseRay((mousePosition), _camera->getCamera());
    if (ray.direction.y == 0)
        return {-1, -1};

    float t = -ray.position.y / ray.direction.y;
    if (t < 0)
        return {-1, -1};

    Vector3 intersection = { ray.position + ray.direction * t };
    int x = static_cast<int>(floor((intersection.x + MAP_REF_TILE_SIZE * 0.5f) / MAP_REF_TILE_SIZE));
    int y = static_cast<int>(floor((intersection.z + MAP_REF_TILE_SIZE * 0.5f) / MAP_REF_TILE_SIZE));

    if (x < 0 || x >= _map->getWidth() || y < 0 || y >= _map->getHeight())
        return {-1, -1};

    return {static_cast<float>(x), static_cast<float>(y)};
}

void Core::moveTile(int directionX, int directionY) {
    if (!_map || !_subwindowManager.get("tile").isVisible())
        return;
    
    int width = _map->getWidth();
    int height = _map->getHeight();
    int x = static_cast<int>(_selectedTileCoords.x);
    int y = static_cast<int>(_selectedTileCoords.y);

    x = (x + directionX + width) % width;
    y = (y + directionY + height) % height;
    _selectedTileCoords = { static_cast<float>(x), static_cast<float>(y) };
    _client->send("bct " + std::to_string(x) + " " + std::to_string(y));
}

float Core::scaleToScreen(float baseValue) const {

    float widthRatio = static_cast<float>(GetScreenWidth()) / static_cast<float>(WINDOW_WIDTH);
    float heightRatio = static_cast<float>(GetScreenHeight()) / static_cast<float>(WINDOW_HEIGHT);

    return baseValue * (widthRatio + heightRatio) * 0.5f;
}

void Core::run() {

    _musicManager.playRandomMusic();

    while (!WindowShouldClose() && !_shouldClose) {
        float deltaTime = GetFrameTime();

        if (_client)
            _client->update();

        checkSetupCamera();

        if (!_camera || !_protocolHandler || !_protocolHandler->isReady()) {
            drawWaitingScreen();
            continue;
        }

        update(deltaTime);
        draw();
    }
    _modelCache.clearCache();
}

void Core::update(float deltaTime) {
    _keyHandler.update(*_camera, _bookHandler->getDroneSpeed());
    if (IsWindowResized())
        _subwindowManager.recomputeAllSubWindows();
    
    _tickAccumulator += deltaTime;
    float tickRate = _timeUnit ? 1.0f / _timeUnit : 0.1f;
    
    _musicManager.update();

    while (_tickAccumulator >= tickRate) {
        _tickAccumulator -= tickRate;
        _tickCounter++;

        if (_tickCounter >= 7) {
             _tickCounter = 0;
            if (_client)
                 _client->send("mct");
        }
    }

    processUI(deltaTime);

    std::vector<int> toRemove;
    for (auto &[id, player] : _players) {
        if (player.isDead() && player.isDeathAnimationFinished())
            toRemove.push_back(id);
        bool visible = _renderer.isVisible(player.getPosition(), *_camera, MAX_VISIBILITY_DISTANCE);
        player.update(deltaTime, visible);
    }
      
    for (int id : toRemove)
        _players.erase(id);

    if (!_players.empty())
        _selectedPlayerIndex = _selectedPlayerIndex % static_cast<int>(_players.size());
    else
        _selectedPlayerIndex = 0;

    for (int y = 0; y < _map->getHeight(); y++)
        for (int x = 0; x < _map->getWidth(); x++)
            _map->getCurrentTile(x, y)->makeItemsFloat();
    _pageFlipSound.updateStream();
}

void Core::drawWaitingScreen() {
    BeginDrawing();
    ClearBackground(BLACK);

    std::string message = "Connecting to server...";
    int fontSize = 20;

    Vector2 textSize = MeasureTextEx(_waitingFont.getFont(), (message).c_str(), fontSize, 1.f);
    Vector2 textPos = {
        (GetScreenWidth() - textSize.x) * 0.5f,
        (GetScreenHeight() - textSize.y) * 0.5f
    };
    DrawText(message.c_str(), static_cast<int>(textPos.x), static_cast<int>(textPos.y), fontSize, WHITE);

    EndDrawing();
}

void Core::updateMarkers() {
    for (auto& [id, player] : _players) {
        if (!_markersEnabled) {
            player.setMarkerVisible(false);
            continue;
        }
        player.setMarkerVisible(true, 1.0f, Player::MarkerShape::DIAMOND);
    }

    if (!_markersEnabled || !_showSelectedTeamOnly || _players.empty()
        || _selectedPlayerIndex >= static_cast<int>(_players.size()))
        return;

    auto it = _players.begin();
    std::advance(it, _selectedPlayerIndex);
    const Player& selected = it->second;
    std::string selectedTeam = selected.getTeam();

    for (auto& [id, player] : _players) {
        if (player.getTeam() != selectedTeam)
            player.setMarkerVisible(false);
        else {
            bool isSelected = (id == it->first);
            float scale = isSelected ? 3.0f : 1.0f;
            auto shape = isSelected ?
                Player::MarkerShape::DOWNARROW : Player::MarkerShape::DIAMOND;
            player.setMarkerVisible(true, scale, shape);
        }
    }
}

void Core::processUI(float deltaTime) {
    for (auto &subWindow : _subwindowManager.getAllActiveWindows()) {
        subWindow.get().update(deltaTime);
        subWindow.get().updateComponents(deltaTime);
    }

    Vector2 mousePos = GetMousePosition();
    bool mousePressed = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    std::string topWindow = _subwindowManager.getFirstWindowInLayerOrder(mousePos);
    bool uiUsed = false;

    if (!topWindow.empty())
        uiUsed = _subwindowManager.get(topWindow).processMouseInput(mousePos, mousePressed);
    else {
        SubWindow &main = _subwindowManager.get("main");

        if (CheckCollisionPointRec(mousePos, main.getRect()))
            uiUsed = main.processMouseInput(mousePos, mousePressed);
    }

    if (mousePressed && !uiUsed)
        handleTileClicks();
}

void Core::handleTileClicks() {
    Vector2 mousePos = {0.f, 0.f};

    if (_keyHandler.isDroneCameraMode())
        mousePos = {static_cast<float>(GetScreenWidth()) * 0.5f,
            static_cast<float>(GetScreenHeight()) * 0.5f};
    else
        mousePos = GetMousePosition();

    std::string topWindow = _subwindowManager.getFirstWindowInLayerOrder(mousePos);
    if (!topWindow.empty() && topWindow != "main")
        return;

    if (!_players.empty() && _camera) {
        Ray ray = GetMouseRay(mousePos, _camera->getCamera());
        int closestPlayerId = -1;
        float closestDistance = std::numeric_limits<float>::max();

        for (const auto& [id, player] : _players) {
            Vector3 center = player.getPosition();
            center.y += 0.5f;
            float radius = 0.6f;
            RayCollision collision = GetRayCollisionSphere(ray, center, radius);
            if (collision.hit) {
                float distance = Vector3Distance(ray.position, collision.point);
                if (distance < closestDistance) {
                    closestDistance = distance;
                    closestPlayerId = id;
                }
            }
        }

        if (closestPlayerId != -1) {
            int index = 0;

            for (const auto& [id, player] : _players) {
                if (id == closestPlayerId) {
                    _selectedPlayerIndex = index;
                    break;
                }
                index++;
            }
            if (!_subwindowManager.get("players").isVisible())
                _actionMap.trigger("togglePlayers");
            _client->send("plv #" + std::to_string(closestPlayerId));
            return;
        }
    }

    Vector2 tilePos = getTileUnderMouse(mousePos);
    if (tilePos.x >= 0 && tilePos.y >= 0) {
        if (_selectedTileCoords.x != tilePos.x || _selectedTileCoords.y != tilePos.y) {
            _selectedTileCoords = tilePos;
            if (!_subwindowManager.get("tile").isVisible()) {
                _client->send("bct " + std::to_string(static_cast<int>(tilePos.x)) + " "
                    + std::to_string(static_cast<int>(tilePos.y)));
                _actionMap.trigger("toggleTile");
            }
        }
    } else if (_subwindowManager.get("tile").isVisible())
            _actionMap.trigger("toggleTile");
}

void Core::draw() {
    BeginDrawing();
    ClearBackground(BLACK);
    {
        BeginMode3D(_camera->getCamera());
        _renderer.drawMap(*_map);

        if (_showGrid)
            _renderer.drawGrid(*_map);
        if (_subwindowManager.get("tile").isVisible() && _selectedTileCoords.x >= 0 && _selectedTileCoords.y >= 0)
            _renderer.drawSelectedTile(*_map, static_cast<int>(_selectedTileCoords.x),
            static_cast<int>(_selectedTileCoords.y));

        std::vector<std::reference_wrapper<const IEntity>> playerRefs;
        playerRefs.reserve(_players.size());

        for (const auto& [id, player] : _players)
            playerRefs.emplace_back(player);
        _renderer.drawEntities(playerRefs,*_camera);

        for (auto& [id, player] : _players)
            player.drawMarker();

        std::vector<std::reference_wrapper<const IEntity>> eggRefs;
        eggRefs.reserve(_eggs.size());

        for (const auto& egg : _eggs)
            eggRefs.emplace_back(*egg);
        _renderer.drawEntities(eggRefs, *_camera);


        std::vector<std::reference_wrapper<const IEntity>> startingEggsRefs;
        startingEggsRefs.reserve(_startingEggs.size());

        for (const auto& egg : _startingEggs)
            startingEggsRefs.emplace_back(*egg);
        _renderer.drawEntities(startingEggsRefs, *_camera);

        EndMode3D();
    }

    drawPanels();

     if (!_winnerTeam.empty()) {
        std::string line1 = "Team " + _winnerTeam + " won...";
        std::string line2 = "Press ESC to quit";

        int fontSize1 = 60;
        int fontSize2 = 30;
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();

        int w1 = MeasureText(line1.c_str(), fontSize1);
        int w2 = MeasureText(line2.c_str(), fontSize2);

        DrawText(line1.c_str(), (screenW - w1) / 2, screenH / 10, fontSize1, GOLD);
        DrawText(line2.c_str(), (screenW - w2) / 2, screenH / 10 + fontSize1 + 10, fontSize2, WHITE);
    }

    EndDrawing();
}

void Core::drawPanels() {
    static const std::vector<std::string> panels = { "main", "chat", "players", "info", "tile" };

    for (const auto &name : panels) {
        SubWindow &window = _subwindowManager.get(name);
        if (!window.isVisible())
            continue;

        window.drawComponents();
    }
}

void Core::initPlayers() {
    
    _playerAnimationHandler.setAnimationMapping({
        {"Run", RUN_MODEL_PATH},
        {"Idle", IDLE_MODEL_PATH},
        {"Stand Up", STANDUP_MODEL_PATH},
        {"Kick", KICK_MODEL_PATH},
        {"Ejected", EJECTED_MODEL_PATH},
        {"Look", LOOK_MODEL_PATH},
        {"Die", DIE_MODEL_PATH},
        {"Pick", PICK_MODEL_PATH},
        {"Incantation", INCANTATION_MODEL_PATH},
        {"Win", WIN_MODEL_PATH}
    });

    std::map<std::string, Raylib::ImageTexture> textures;
    textures.emplace("Level 1", LEVEL1_TEXTURE_PATH);
    textures.emplace("Level 2", LEVEL2_TEXTURE_PATH);
    textures.emplace("Level 3", LEVEL3_TEXTURE_PATH);
    textures.emplace("Level 4", LEVEL4_TEXTURE_PATH);
    textures.emplace("Level 5", LEVEL5_TEXTURE_PATH);
    textures.emplace("Level 6", LEVEL6_TEXTURE_PATH);
    textures.emplace("Level 7", LEVEL7_TEXTURE_PATH);
    textures.emplace("Level 8", LEVEL8_TEXTURE_PATH);
    _playerAnimationHandler.setTextureCache(std::move(textures));

    _playerAnimationHandler.cacheAnimations();
}

std::optional<std::reference_wrapper<const Player>> Core::getSelectedPlayer() const {
    if (_players.empty() || _selectedPlayerIndex < 0 || _selectedPlayerIndex >= static_cast<int>(_players.size()))
        return std::nullopt;
    auto it = _players.begin();
    std::advance(it, _selectedPlayerIndex);
    return it->second;
}

void Core::onMsz(int width, int height) {
    if (_isMapReady) {
        std::cout << "Map already initialized." << std::endl;
        return;
    }

    _map->resize(width, height, _mapDensity);
    _isMapReady = true;
    _selectedTileCoords = {0.0f, 0.0f};
}

void Core::onBct(int x, int y, const std::array<int, RESOURCE_COUNT>& resources) {
    if (x < 0 || x >= _map->getWidth() || y < 0 || y >= _map->getHeight()) {
        std::cerr << "Tile data out of bounds: (" << x << "," << y << ")" << std::endl;
        return;
    }

    int foodCount = _map->getCurrentTile(x, y)->getFoodCount();
    int foodDiff = resources[0] - foodCount;

    if (foodDiff > 0) {
        for (int i = 0; i <= foodDiff - 1; i++)
            _map->getCurrentTile(x, y)->placeFood();
    } else {
        for (int i = 0; i < -foodDiff; i++)
            _map->getCurrentTile(x, y)->removeFood();
    }

    std::map<StoneType, int> stoneTypeCount = _map->getCurrentTile(x, y)->getStoneCounts();
    std::vector<int> stoneDiffs(6);
    for (int i = 0; i < 6; i++)
        stoneDiffs[i] = resources[i + 1] - stoneTypeCount[static_cast<StoneType>(i)];

    for (int i = 0; i < 6; i++) {
        if (stoneDiffs[i] > 0)
            for (int j = 0; j < stoneDiffs[i]; j++)
                _map->getCurrentTile(x, y)->placeStone(static_cast<StoneType>(i));
        else
            for (int j = 0; j < -stoneDiffs[i]; j++)
                _map->getCurrentTile(x, y)->removeStone(static_cast<StoneType>(i));
    }
}

void Core::onSgt(int timeUnit) {
    _timeUnit = timeUnit;
    _bookHandler->setFrequency(_timeUnit);
    for (auto& [id, player] : _players)
        player.setTimeUnit(_timeUnit);
}

void Core::onTna(const std::string& teamName) {
    if (std::find(_teamNames.begin(), _teamNames.end(), teamName) == _teamNames.end())
        _teamNames.push_back(teamName);
    else
        std::cerr << "Can't create this team, team name already exists: " << teamName << std::endl;
}

void Core::onPnw(int id, int x, int y, int orientation, int level, const std::string& team) {
    if (x < 0 || x >= _map->getWidth() || y < 0 || y >= _map->getHeight()) {
        std::cerr << "pnw: Player position out of bounds: (" << x << "," << y << ")" << std::endl;
        return;
    }

    if (_players.size() == 1)
        _selectedPlayerIndex = 0;

    if (orientation < 1 || orientation > 4) {
        std::cerr << "pnw: invalid orientation " << orientation << std::endl;
        return;
    }

    Color teamColor = getTeamColor(team);
    _players.try_emplace(id,
        id, team, IDLE_MODEL_PATH,
        Vector2{static_cast<float>(x), static_cast<float>(y)},
        Vector3{-90.f, 0.f, 0.f}, 0.01f, *_map, _playerAnimationHandler, _timeUnit, _modelCache
    );
    _players.at(id).setTeamColor(teamColor);
    _players.at(id).setLevel(level);
    updateMarkers();
}

void Core::onPpo(int id, int x, int y, int orientation) {
    if (x < 0 || x >= _map->getWidth() || y < 0 || y >= _map->getHeight()) {
        std::cerr << "ppo: New player position out of bounds: (" << x << "," << y << ") can't move there" << std::endl;
        return;
    }

    if (orientation < 1 || orientation > 4) {
        std::cerr << "ppo: invalid orientation " << orientation << std::endl;
        return;
    }

    if (_players.find(id) == _players.end()) {
        std::cerr << "ppo: unknown player id " << id << std::endl;
        return;
    }

    Vector2 newTile{static_cast<float>(x), static_cast<float>(y)};
    Vector2 currentTile = _players.at(id).getCurrentTile();

     if (_players.at(id).isDead())
        return;

    if (newTile.x != currentTile.x || newTile.y != currentTile.y)
        _players.at(id).setTargetTile(newTile, orientation);
}

void Core::onPlv(int id, int level) {

    if (_players.find(id) == _players.end()) {
        std::cerr << "plv: unknown player id " << id << std::endl;
        return;
    }

    if (_players.at(id).getLevel() != level)
        _players.at(id).levelUp();
}

void Core::onPin(int id, const std::array<int, RESOURCE_COUNT>& inventory) {
    if (_players.find(id) == _players.end()) {
        std::cerr << "pin: unknown player id " << id << std::endl;
        return;
    }

    if (_players.at(id).isDead())
        return;
    _players.at(id).setInventory(inventory);
}

void Core::onPdi(int id) {

    if (_players.find(id) == _players.end())
        return;
    if (_players.at(id).isDead())
        return;

    _players.at(id).playAnimation("Die");
    _numberDeadPlayers++;
    _players.at(id).stopIncantation();
}

void Core::onEnw(int eggId, int playerId, int x, int y) {
    if (x < 0 || x >= _map->getWidth() || y < 0 || y >= _map->getHeight()) {
        std::cerr << "enw: Egg position out of bounds: (" << x << "," << y << ")" << std::endl;
        return;
    }

    
    Vector3 actualPosition = {
        static_cast<float>(x) * MAP_REF_TILE_SIZE,
        0.f,
        static_cast<float>(y) * MAP_REF_TILE_SIZE
    };
    
    if (playerId == -1) {
        _startingEggs.push_back(std::make_unique<Egg>(eggId, "NULL", actualPosition, _playerAnimationHandler, _timeUnit, _modelCache));
        return;
    }

    if (_players.find(playerId) == _players.end()) {
        std::cerr << "enw: unknown player id " << playerId << std::endl;
        return;
    }

    _eggs.emplace_back(std::make_unique<Egg>(eggId, _players.at(playerId).getTeam(), actualPosition, _playerAnimationHandler, _timeUnit, _modelCache));

}

void Core::onEbo(int eggId) {

    for (int i = 0; i < static_cast<int>(_startingEggs.size()); i++) {
        if (_startingEggs[i]->getId() == eggId) {
            _startingEggs.erase(_startingEggs.begin() + i);
            break;
        }
    }

    for (int i = 0; i < static_cast<int>(_eggs.size()); i++) {
        if (_eggs[i]->getId() == eggId) {
            _eggs.erase(_eggs.begin() + i);
            break;
        }
    }
}

void Core::onEdi(int eggId) {
    for (int i = 0; i < static_cast<int>(_eggs.size()); i++) {
        if (_eggs[i]->getId() == eggId) {
            _eggs.erase(_eggs.begin() + i);
            break;
        }
    }

    for (int i = 0; i < static_cast<int>(_startingEggs.size()); i++) {
        if (_startingEggs[i]->getId() == eggId) {
            _startingEggs.erase(_startingEggs.begin() + i);
            break;
        }
    }
}

void Core::onPex(int id) {

    if (_players.find(id) == _players.end()) {
        std::cerr << "pex: unknown player id " << id << std::endl;
        return;
    }

    Player &player = _players.at(id);
    Vector2 currentTile = player.getCurrentTile();
    
    player.playAnimation("Kick");

    for (auto &[otherId, otherPlayer] : _players) {
        if (otherId == id)
            continue;
        if (otherPlayer.getCurrentTile().x == currentTile.x &&
            otherPlayer.getCurrentTile().y == currentTile.y) {
            otherPlayer.playAnimation("Ejected");
            otherPlayer.startEject(player.getDirection());
        }
    }
}

void Core::onPbc(int id, const std::string& message) {
    if (_chatPanel) {
        std::string team = "Unknown";

        auto it = _players.find(id);
        if (it != _players.end())
            team = it->second.getTeam();

        _chatPanel->addBroadcast(id, team, message);
    }
}

void Core::onPic(const std::vector<int>& players) {

    if (_players.find(players.at(0)) == _players.end()) {
        std::cerr << "enw: unknown player id " << players.at(0) << std::endl;
        return;
    }

    for (int id : players) {
        if (_players.find(id) == _players.end()) {
            std::cerr << "pic: unknown player id " << id << std::endl;
            continue;
        }
        _players.at(id).startIncantation();
    }

}

void Core::onPie(int x, int y, int result) {
    if (!result)
        return;

    for (auto& [id, player] : _players) {
        Vector2 tile = player.getCurrentTile();
        if (static_cast<int>(tile.x) == x && static_cast<int>(tile.y) == y) {
            player.stopIncantation();
            _client->send("plv #" + std::to_string(id));
        }
    }

}

void Core::onPfk(int id) {

    if (_players.find(id) == _players.end()) {
        std::cerr << "pfk: unknown player id " << id << std::endl;
        return;
    }
}

void Core::onPdr(int id, int resource) {
    if (_players.find(id) == _players.end()) return;

    Player &player = _players.at(id);
    Vector2 tile = player.getCurrentTile();

    if (player.getInventory()[resource] > 0) {
        player.getInventory()[resource]--;
        if (resource == 0)
            _map->getCurrentTile(tile.x, tile.y)->placeFood();
        else
            _map->getCurrentTile(tile.x, tile.y)->placeStone(static_cast<StoneType>(resource - 1));

        _client->send("bct " + std::to_string(static_cast<int>(tile.x)) + " " + std::to_string(static_cast<int>(tile.y)));
    }
}

void Core::onPgt(int id, int resource) {
    if (_players.find(id) == _players.end())
        return;

    Player &player = _players.at(id);
    Vector2 tile = player.getCurrentTile();

    player.playAnimation("Pick");

    if (resource == 0) {
        _map->getCurrentTile(tile.x, tile.y)->removeFood();
        player.getInventory()[0]++;
    } else {
        _map->getCurrentTile(tile.x, tile.y)->removeStone(static_cast<StoneType>(resource - 1));
        player.getInventory()[resource]++;
    }

    _client->send("bct " + std::to_string(static_cast<int>(tile.x)) + " " + std::to_string(static_cast<int>(tile.y)));
}

void Core::onSeg(const std::string& teamName) {

    for (auto it = _players.begin(); it != _players.end(); ) {
        if (it->second.getTeam() != teamName)
            it = _players.erase(it);
        else
            it++;
    }

    for (auto it = _eggs.begin(); it != _eggs.end(); )
        it = _eggs.erase(it);

    for (auto it = _startingEggs.begin(); it != _startingEggs.end(); )
        it = _startingEggs.erase(it);

    for (auto &[id, player] : _players)
        player.playAnimation("Win");

    _winnerTeam = teamName;
    _musicManager.playMusic("Macarena");
}

void Core::onSst(int timeUnit) {
    _timeUnit = timeUnit;
    _bookHandler->setFrequency(_timeUnit);
    for (auto& [id, player] : _players)
        player.setTimeUnit(_timeUnit);
}

void Core::onSmg(const std::string& message) {
    std::cout << "Server message: " << message << std::endl;
}

}
