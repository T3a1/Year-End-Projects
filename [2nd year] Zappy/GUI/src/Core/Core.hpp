/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** Core
*/

#pragma once
#include "ViewCamera.hpp"
#include "Map.hpp"
#include "KeyHandler.hpp"
#include "ActionMap.hpp"
#include "SubWindowManager.hpp"
#include "Button.hpp"
#include "Player.hpp"
#include "Egg.hpp"
#include "ContainerComponent.hpp"
#include "BookHandler.hpp"
#include "Client.hpp"
#include "ProtocolHandler.hpp"
#include "Renderer.hpp"
#include "PlayerPanelHandler.hpp"
#include "TilePanelHandler.hpp"
#include "ChatPanelHandler.hpp"
#include <unordered_map>
#include <vector>
#include <memory>
#include <list>
#include <set>
#include <unordered_set>
#include "TextFont.hpp"
#include "optional"
#include "MusicManager.hpp"
#include "GameInfoComponent.hpp"

namespace Zappy {
    class Core {
        public:

            Core (float mapDensity);
            ~Core();
            Core(const Core &core) = delete;
            Core &operator=(const Core &core) = delete;

            void run();
            void switchPage(const std::string &pageName, bool playSound);
            void initConnection(const std::string &host, int port);
            bool isAnyPanelOpen() const;

            void onMsz(int width, int height);
            void onBct(int x, int y, const std::array<int, RESOURCE_COUNT>& resources);
            void onSgt(int timeUnit);
            void onTna(const std::string& teamName);
            void onNew(int id, int x, int y, int orientation, int level, const std::string& team);
            void onPnw(int id, int x, int y, int orientation, int level, const std::string& team);
            void onPpo(int id, int x, int y, int orientation);
            void onPlv(int id, int level);
            void onPin(int id, const std::array<int, RESOURCE_COUNT>& inventory);
            void onPdi(int id);
            void onEnw(int eggId, int playerId, int x, int y);
            void onEbo(int eggId);
            void onEdi(int eggId);
            void onPex(int id);
            void onPbc(int id, const std::string& message);
            void onPic(const std::vector<int>& players);
            void onPie(int x, int y, int result);
            void onPfk(int id);
            void onPdr(int id, int resource);
            void onPgt(int id, int resource);
            void onSeg(const std::string& teamName);
            void onSst(int timeUnit);
            void onSmg(const std::string& message);
            
            int getTimeUnit() const { return _timeUnit; }
            Color getTeamColor(const std::string& teamName);
            GameInfo getGameInfo() const;
            
        private:
            void update(float deltaTime);
            void draw();
            void drawWaitingScreen();
            void drawPanels();
            void registerActions();
            void processUI(float deltaTime);
            void handleTileClicks();
            void updateMarkers();

            void initSubWindows();
            void initSubWindowsContent();
            void initButtons();
            void initMainWindow();
            void initPlayersWindow();
            void initTileWindow();
            void initPlayers();
            void initChatWindow();
            void initInfoWindow();
            void initMainButtons();
            void initInfoButtons();
            void checkSetupCamera();
            Vector2 getTileUnderMouse(const Vector2& mousePosition) const;
            void moveTile(int directionX, int directionY);
            float scaleToScreen(float baseValue) const;
            std::optional<std::reference_wrapper<const Player>> getSelectedPlayer() const;


            bool _shouldClose;
            bool _showGrid = true;
            std::unique_ptr<Raylib::ViewCamera> _camera;
            Renderer _renderer;
            ActionMap _actionMap;
            KeyHandler _keyHandler;
            SubWindowManager _subwindowManager;
            float _tickAccumulator;
            std::unordered_map<std::string, std::unordered_map<std::string, std::reference_wrapper<Button>>> _buttons;
            std::unordered_map<std::string, std::reference_wrapper<Button>> _linksButtonWindow;
            std::unordered_map<int, Player> _players;
            std::vector<std::unique_ptr<Egg>> _startingEggs;
            std::vector<std::unique_ptr<Egg>> _eggs;
            AnimationHandler _playerAnimationHandler;
            std::unique_ptr<BookHandler> _bookHandler;

            std::unique_ptr<Client> _client;
            std::unique_ptr<ProtocolHandler> _protocolHandler;

            std::vector<std::string> _teamNames;
            int _timeUnit;
            bool _isMapReady;

            ModelCache _modelCache;
            std::unique_ptr<PlayerPanelHandler> _playerPanel;
            int _selectedPlayerIndex = 0;
            std::unique_ptr<TilePanelHandler> _tilePanel;
            Vector2 _selectedTileCoords = {0, 0};
            std::unique_ptr<ChatPanelHandler> _chatPanel;
            std::unique_ptr<Map> _map;
            float _mapDensity;

            std::unordered_map<std::string, Color> _teamColors;
            bool _markersEnabled = true;
            bool _showSelectedTeamOnly = false;
            int _tickCounter = 0;
            int _numberDeadPlayers = 0;

            Raylib::TextFont _waitingFont;
            MusicManager _musicManager;

            std::string _winnerTeam;

            Raylib::MusicSound _pageFlipSound;
    };
}
