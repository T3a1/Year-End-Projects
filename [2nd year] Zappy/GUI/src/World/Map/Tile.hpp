/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** Tile
*/

#ifndef TILE_HPP_
#define TILE_HPP_

#include "Food.hpp"
#include "Stone.hpp"
#include <vector>
#include <memory>
#include <map>
#include "Constants.hpp"

namespace Zappy {
    class Tile {
        public:
            Tile(Vector3 position, Vector3 worldPosition, ModelCache& modelCache);
            ~Tile();

            const std::vector<std::unique_ptr<Food>>& getFood() const { return _food; }
            int getFoodCount() const { return _food.size(); }
            const std::vector<std::unique_ptr<Stone>>& getStones() const { return _stones; }
            int getStoneCount() const { return _stones.size(); }
            std::map<StoneType, int> getStoneCounts() const { return _stonesCount; };

            Vector3 getPosition() const { return _position; }
            Vector3 getWorldPosition() const { return _worldPosition; }

            void placeFood();
            std::unique_ptr<Food> removeFood();

            void placeStone(StoneType type);
            std::string getStoneModelPath(StoneType type) const;
            std::unique_ptr<Stone> removeStone(StoneType type);

            void makeItemsFloat();

        protected:
        private:

        std::vector<std::unique_ptr<Food>> _food;
        std::vector<std::unique_ptr<Stone>> _stones;
        std::map<StoneType, int> _stonesCount;
        Vector3 _position;
        Vector3 _worldPosition;
        ModelCache& _modelCache;
    };
}

#endif /* !TILE_HPP_ */
