/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** Tile
*/

#include "Tile.hpp"

namespace Zappy {
    Tile::Tile(Vector3 position, Vector3 worldPosition, ModelCache& modelCache)
        : _stonesCount({
            {StoneType::LINEMATE, 0}, {StoneType::DERAUMERE, 0}, {StoneType::SIBUR, 0},
            {StoneType::MENDIANE, 0}, {StoneType::PHIRAS, 0}, {StoneType::THYSTAME, 0}
        }),
        _position(position),
        _worldPosition(worldPosition),
        _modelCache(modelCache)
    {
    }

    Tile::~Tile()
    {
    }


    void Tile::placeFood()
    {
        float offsetX = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
        float offsetZ = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;

        Vector3 randomPosition = {
            _worldPosition.x + offsetX,
            _worldPosition.y,
            _worldPosition.z + offsetZ
        };

        float randomYRotation = static_cast<float>(rand() % 360);
        Vector3 randomRotation = { 0.f, randomYRotation, 0.f };

        std::unique_ptr<Food> food = std::make_unique<Food>(FOOD_MODEL_PATH,
            randomPosition, randomRotation, 0.06f, _modelCache);

        _food.push_back(std::move(food));
    }

    std::unique_ptr<Food> Tile::removeFood()
    {
        if (_food.empty())
            return nullptr;

        std::unique_ptr<Food> food = std::move(_food.back());
        _food.pop_back();
        return food;
    }

    void Tile::placeStone(StoneType type)
    {
        float offsetX = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
        float offsetZ = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;

        Vector3 randomPosition = {
            _worldPosition.x + offsetX,
            _worldPosition.y,
            _worldPosition.z + offsetZ
        };

        float randomYRotation = static_cast<float>(rand() % 360);
        Vector3 randomRotation = { 0.f, randomYRotation, 0.f };

        std::unique_ptr<Stone> stone = std::make_unique<Stone>(getStoneModelPath(type),
            randomPosition, randomRotation, 0.24f, type, _modelCache);

        _stones.push_back(std::move(stone));
        _stonesCount[type]++;
    }

    std::string Tile::getStoneModelPath(StoneType type) const
    {
        switch (type) {
            case StoneType::LINEMATE:
                return LINEMATE_MODEL_PATH;
            case StoneType::DERAUMERE:
                return DERAUMERE_MODEL_PATH;
            case StoneType::SIBUR:
                return SIBUR_MODEL_PATH;
            case StoneType::MENDIANE:
                return MENDIANE_MODEL_PATH;
            case StoneType::PHIRAS:
                return PHIRAS_MODEL_PATH;
            case StoneType::THYSTAME:
                return THYSTAME_MODEL_PATH;
            default:
                return "";
        }
    }


    std::unique_ptr<Stone> Tile::removeStone(StoneType type)
    {
        if (_stones.empty())
            return nullptr;

        for (auto it = _stones.begin(); it != _stones.end(); it++) {
            if ((*it)->getType() == type) {
                std::unique_ptr<Stone> stone = std::move(*it);
                _stones.erase(it);
                _stonesCount[type]--;
                return stone;
            }
        }
        return nullptr;
    }

    void Tile::makeItemsFloat()
    {
        float bobAmplitude = 0.12f;

        for (auto &food : _food) {
            float offset = (sinf(GetTime() * 2.0f + food->getBobPhase()) + 1.0f) * 0.5f * bobAmplitude;
            Vector3 position = food->getPosition();
            position.y = _worldPosition.y + offset;
            food->setPosition(position);
        }

        for (auto &stone : _stones) {
            float offset = (sinf(GetTime() * 2.0f + stone->getBobPhase()) + 1.0f) * 0.5f * bobAmplitude;
            Vector3 position = stone->getPosition();
            position.y = _worldPosition.y + offset;
            stone->setPosition(position);
        }
    }
}
