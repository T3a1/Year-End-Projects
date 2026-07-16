/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** BookHandler
*/

#pragma once
#include "ContainerComponent.hpp"
#include "ActionMap.hpp"
#include <unordered_map>
#include <functional>
#include <string>
#include <iostream>
#include "RectangleImage.hpp"
#include "Button.hpp"
#include "MusicManager.hpp"
#include "TextFont.hpp"
#include "TextComponent.hpp"

namespace Zappy {
    class SubWindow;
    class Core;

    enum WindowMode {
        FULLSCREEN,
        WINDOWED
    };
    struct SettingsPageData {
        WindowMode windowMode = FULLSCREEN;
        float volume = 50.0f;
        float cameraSpeed = 0.3f;
        int frequency = 1;
    };

    class BookHandler {
        public:
            BookHandler(ActionMap &actionMap, MusicManager &musicManager, Core& core);
            void init(SubWindow &infoWindow);
            void switchPage(const std::string &pageName, SubWindow &infoWindow);
            const std::string& getCurrentPage() const { return _currentPage; }

            void registerBookActions();

            void buildAboutPage(ContainerComponent &container, SubWindow &infoWindow);
            void buildSettingsPage(ContainerComponent &container, SubWindow &infoWindow);
            void buildInputsPage(ContainerComponent &container, SubWindow &infoWindow);
            void buildCreditsPage(ContainerComponent &container, SubWindow &infoWindow);

            float getDroneSpeed() const { return _settingsData.cameraSpeed; }

            void setFrequency(int frequency);

        private:
            std::shared_ptr<ContainerComponent> _pageContainer;
            std::unordered_map<std::string, std::function<void(ContainerComponent&, SubWindow&)>> _pageBuilders;
            std::string _currentPage;
            ActionMap &_actionMap;
            MusicManager &_musicManager;
            SettingsPageData _settingsData;

            std::map<std::string, std::shared_ptr<Button>> _settingsButtons;
            std::shared_ptr<TextComponent> _frequencyText;
            Raylib::TextFont _font;

            Core& _core;
    };
}
