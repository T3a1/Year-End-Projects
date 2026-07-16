/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** TileInfoComponent
*/

#pragma once
#include "ASubWindowComponent.hpp"
#include "Tile.hpp"
#include "Constants.hpp"
#include "ImageTexture.hpp"
#include <functional>
#include <array>

namespace Zappy {
    class TileInfoComponent : public ASubWindowComponent {
        public:
            TileInfoComponent(std::function<const Tile*()> getTile,
                float x = 0.5f, float y = 0.5f, float width = 1.f,
                float height = 1.f, int layer = 0);
            ~TileInfoComponent() override;

            void draw(const Rectangle& parentRect) const override;
            void update(float) override {}

        private:
            std::function<const Tile*()> _getTile;
            std::map<std::string, std::string> _resourceIcons = {
                {"Food", INFO_FOOD_TEXTURE_PATH},
                {"Linemate", INFO_LINEMATE_TEXTURE_PATH},
                {"Deraumere", INFO_DERAUMERE_TEXTURE_PATH},
                {"Sibur", INFO_SIBUR_TEXTURE_PATH},
                {"Mendiane", INFO_MENDIANE_TEXTURE_PATH},
                {"Phiras", INFO_PHIRAS_TEXTURE_PATH},
                {"Thystame", INFO_THYSTAME_TEXTURE_PATH}
            };

            std::map<std::string, Raylib::ImageTexture> _resourceTextures;
    };
}
