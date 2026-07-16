/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** menu
*/

#ifndef MENU_HPP_
#define MENU_HPP_

#include "SubWindowManager.hpp"
#include "Button.hpp"
#include "ActionMap.hpp"
#include "Errors.hpp"
#include <map>
#include "RectangleImage.hpp"
#include "MusicSound.hpp"
#include "Constants.hpp"
#include "TextFont.hpp"
#include "Renderer.hpp"
#include "Rat.hpp"
#include "Client.hpp"

namespace Zappy {

    class Menu {
        public:
            Menu(std::string port, std::string machine);
            ~Menu();
            void initImages();
            void initButtons();
            void initRats();
            void drawTexts();
            void display();
            void registerActions();
            void editTextField();
            void processUI(float deltaTime);
            Rectangle getCenterPosition(float x, float y, float width, float height) const;

            const std::string& getPort() const { return _port; }
            const std::string& getMachine() const { return _machine; }
            bool shouldQuit() const { return _quitMenu; }
            void recomputeImages();
            bool tryToConnect();

        protected:
        private:
            SubWindowManager _subwindowManager;
            ActionMap _actionMap;

            std::map<std::string, std::reference_wrapper<Button>> _buttons;

            std::string _port;
            std::string _machine;

            std::string _currentEditingField;
            bool _lockedFields;

            bool _inMenu = true;
            bool _quitMenu = false;
            bool _correctValues = true;

            std::vector<Zappy::Rat::ratPosition> _ratPositions;
            
            Raylib::MusicSound _menuMusic;
            Raylib::TextFont _font;        protected:
            std::map<std::string, Raylib::RectangleImage> _images;
            Zappy::Rat::ratInfo _rat;

            Renderer _renderer;

    };
}

#endif /* !MENU_HPP_ */
