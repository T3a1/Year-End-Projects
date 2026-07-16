/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Renderer
*/

#pragma once
#include "Map.hpp"
#include "SubWindow.hpp"
#include "IEntity.hpp"
#include <list>
#include "ViewCamera.hpp"
#include "RectangleImage.hpp"
#include "TextFont.hpp"
#include "Rat.hpp"

namespace Zappy {
    class Renderer {
        public: 
            Renderer();
            ~Renderer() = default;

            void drawMap(const Map &map) const;
            float scaledTileSize() const;
            void drawEntities(const std::vector<std::reference_wrapper<const IEntity>> &entities, const Raylib::ViewCamera &camera) const;
            bool isVisible(const Vector3 &position, const Raylib::ViewCamera &camera, float maxDistance) const;
            Color getBgColor() const { return _bgColor; }
            void drawGrid(const Map &map) const;
            float scaledFontSize(float baseFontSize) const;

            void drawMenuImages(const std::map<std::string, Raylib::RectangleImage>& images, bool lockedFields) const;
            void drawMenuTexts(const SubWindow& menuWindow, Raylib::TextFont& font,
                const std::string& port, const std::string& machine, bool correctValues) const;
            void drawMenuRats(const Rat::ratInfo& rat, const std::vector<Zappy::Rat::ratPosition>& ratPositions,
                const SubWindow& menuWindow) const;
            void drawSelectedTile(const Map &map, int x, int y) const;

        private:
            void drawGround(const Map &map) const;

            Color _tileColor;
            Color _wireColor;
            Color _bgColor;
    };
}
