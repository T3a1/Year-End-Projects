/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** PlayerInfoComponent
*/

#pragma once
#include "ASubWindowComponent.hpp"
#include "Player.hpp"
#include <functional>
#include <array>

namespace Zappy {
    class PlayerInfoComponent : public ASubWindowComponent {
        public:
            PlayerInfoComponent(std::function<const Player*()> getPlayer,
                float x = 0.4f, float y = 0.4f, float width = 0.f, float height = 0.f, int layer = 0);
            ~PlayerInfoComponent() override;

            void draw(const Rectangle& parentRect) const override;
            void update(float) override {}

        private:
            void renderPlayerPreview(const Player& player) const;
            mutable RenderTexture2D _previewTexture;
            mutable Camera3D _previewCamera;

            std::function<const Player*()> _getPlayer;
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
