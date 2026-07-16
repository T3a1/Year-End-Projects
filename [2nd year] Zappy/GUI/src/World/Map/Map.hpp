/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Map
*/

#pragma once
#include <vector>
#include "map"
#include "Constants.hpp"
#include "Tile.hpp"
#include "Prop.hpp"
#include <algorithm>
#include "ImageTexture.hpp"

namespace Zappy {

    enum propType {
        ROCK_1,
        ROCK_2,
        ROCK_3,
        TREE_1,
        TREE_2,
        TREE_3,
        TREE_4,
        TREE_5,
        TREE_6,
        BUSH_1,
        LENGTH
    };

    class Map {
        public:
            Map(int width, int height, ModelCache& modelCache);
            Map(const Map &) = delete;
            Map &operator=(const Map &) = delete;
            Map(Map &&) noexcept = default;
            Map &operator=(Map &&) noexcept = delete;
            ~Map() = default;
        
            int getWidth() const { return _width; }
            int getHeight() const { return _height; }
            const std::vector<std::unique_ptr<Tile>>& getTiles() const { return _tiles; }
            const Model &getGroundModel() const { return _groundModel; }
            const std::vector<std::unique_ptr<Prop>>& getProps() const { return _props; }

            void createProps(float density);
            Vector3 generateRandomPosition();
            void calculateScaleAndRotation(const Vector3 &position, float &scale, Vector3 &rotation, float maxDist, float scaleMin, float scaleMax);
            void sortPropsByDistance(std::vector<std::unique_ptr<Prop>>& props);

            std::string getPropModelPath(propType type) const;

            std::unique_ptr<Tile> &getCurrentTile(int x, int y);
            const std::unique_ptr<Tile> &getCurrentTile(int x, int y) const;

            void resize(int newWidth, int newHeight, float mapDensity);

        private:
            int _width;
            int _height;
            float _planeSize;

            std::vector<std::unique_ptr<Tile>> _tiles;
            Model _groundModel;

            std::vector<std::unique_ptr<Prop>> _props;
            ModelCache& _modelCache;

            Raylib::ImageTexture _grassTexture;
    };
}
