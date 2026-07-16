/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Map
*/

#include "Map.hpp"
#include "Errors.hpp"

namespace Zappy {

    Map::Map(int width, int height, ModelCache& modelCache)
        : _width(width), _height(height), _modelCache(modelCache), _grassTexture(GRASS_TEXTURE_PATH) {
        if (width <= 0 || height <= 0)
            throw ValidationError("Map dimensions must be positive");

        _tiles.reserve(width * height);
        
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Vector3 gridPosition = { static_cast<float>(x), 0.f, static_cast<float>(y) };

                Vector3 position = {
                    x * MAP_REF_TILE_SIZE,
                    0.f,
                    y * MAP_REF_TILE_SIZE
                };
                _tiles.emplace_back(std::make_unique<Tile>(gridPosition, position, _modelCache));
            }
        }

        GenTextureMipmaps(&_grassTexture.getTexture());
        SetTextureWrap(_grassTexture.getTexture(), TEXTURE_WRAP_REPEAT);
        SetTextureFilter(_grassTexture.getTexture(), TEXTURE_FILTER_TRILINEAR);

        _planeSize = 750.0f;
        Mesh groundMesh = GenMeshPlane(_planeSize, _planeSize, 1, 1);

        float tileRepeat = 150.0f;
        for (int i = 0; i < groundMesh.vertexCount; i++) {
            groundMesh.texcoords[i * 2] *= tileRepeat;
            groundMesh.texcoords[i * 2 + 1] *= tileRepeat;
        }
        UpdateMeshBuffer(groundMesh, 1, groundMesh.texcoords, groundMesh.vertexCount * 2 * sizeof(float), 0);

        _groundModel = LoadModelFromMesh(groundMesh);
        _groundModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = _grassTexture.getTexture();
    }

    std::unique_ptr<Tile> &Map::getCurrentTile(int x, int y) {
        return _tiles.at(y * _width + x);
    }

    const std::unique_ptr<Tile> &Map::getCurrentTile(int x, int y) const {
        return _tiles.at(y * _width + x);
    }

    void Map::calculateScaleAndRotation(const Vector3 &position, float &scale, Vector3 &rotation, float maxDist, float scaleMin, float scaleMax)
    {
        float mapMinX = 0.f;
        float mapMaxX = _width * MAP_REF_TILE_SIZE;
        float mapMinZ = 0.f;
        float mapMaxZ = _height * MAP_REF_TILE_SIZE;

        float dx = 0.f;
        if (position.x < mapMinX)
            dx = mapMinX - position.x;
        else if (position.x > mapMaxX)
            dx = position.x - mapMaxX;

        float dz = 0.f;
        if (position.z < mapMinZ)
            dz = mapMinZ - position.z;
        else if (position.z > mapMaxZ)
            dz = position.z - mapMaxZ;

        float dist = std::sqrt(dx * dx + dz * dz);
        float distanceFactor = std::min(dist / maxDist, 1.f);
        float baseScale = scaleMin + distanceFactor * (scaleMax - scaleMin);
        float noise = 0.7f + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 0.6f;
        scale = baseScale * noise;

        rotation = { 0.f, static_cast<float>(rand() % 360), 0.f };
    }

    Vector3 Map::generateRandomPosition()
    {
        float x = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * _planeSize - _planeSize / 2.f;
        float z = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * _planeSize - _planeSize / 2.f;
        return {x, 0.f, z};
    }
    
    void Map::sortPropsByDistance(std::vector<std::unique_ptr<Prop>>& props)
    {
        std::sort(props.begin(), props.end(),
            [](const std::unique_ptr<Prop>& a, const std::unique_ptr<Prop>& b)
            {
                const Vector3 posA = a->getPosition();
                const Vector3 posB = b->getPosition();

                const float distASq = posA.x * posA.x + posA.z * posA.z;
                const float distBSq = posB.x * posB.x + posB.z * posB.z;

                return distASq > distBSq;
            });
    }


    void Map::createProps(float density)
    {
        int totalProps = static_cast<int>(_planeSize * _planeSize * density);
        float maxDist = _planeSize * 0.7f;
        float scaleMin = 0.8f;
        float scaleMax = 15.0f;

        float mapMinX = 0.f;
        float mapMaxX = _width * MAP_REF_TILE_SIZE;
        float mapMinZ = 0.f;
        float mapMaxZ = _height * MAP_REF_TILE_SIZE;

        int attempts = 0;
        int placed = 0;

        while (placed < totalProps && attempts < totalProps * 10) {
            Vector3 position = generateRandomPosition();

            bool insideTileMap = (position.x >= mapMinX && position.x <= mapMaxX &&
                position.z >= mapMinZ && position.z <= mapMaxZ);
            if (insideTileMap) {
                attempts++;
                continue;
            }

            float scale;
            Vector3 rotation;
            calculateScaleAndRotation(position, scale, rotation, maxDist, scaleMin, scaleMax);

            propType type = static_cast<propType>(rand() % static_cast<int>(propType::LENGTH));
            _props.emplace_back(std::make_unique<Prop>(
                getPropModelPath(type), position, rotation, scale, _modelCache));

            placed++;
            attempts++;
        }

        sortPropsByDistance(_props);
        
    }

    std::string Map::getPropModelPath(propType type) const
    {
        switch (type) {
            case ROCK_1:
                return ROCK1_MODEL_PATH;
            case ROCK_2:
                return ROCK2_MODEL_PATH;
            case ROCK_3:
                return ROCK3_MODEL_PATH;
            case TREE_1:
                return TREE1_MODEL_PATH;
            case TREE_2:
                return TREE2_MODEL_PATH;
            case TREE_3:
                return TREE3_MODEL_PATH;
            case TREE_4:
                return TREE4_MODEL_PATH;
            case TREE_5:
                return TREE5_MODEL_PATH;
            case TREE_6:
                return TREE6_MODEL_PATH;
            case BUSH_1:
                return BUSH1_MODEL_PATH;
            default:
                return ROCK1_MODEL_PATH;
        }
    }

    void Map::resize(int newWidth, int newHeight, float mapDensity) {
        _tiles.clear();
        _width = newWidth;
        _height = newHeight;
        _tiles.reserve(newWidth * newHeight);

        for (int y = 0; y < newHeight; y++) {
            for (int x = 0; x < newWidth; x++) {
                Vector3 gridPosition = { static_cast<float>(x), 0.f, static_cast<float>(y) };
                Vector3 position = { static_cast<float>(x) * MAP_REF_TILE_SIZE, 0.f, static_cast<float>(y) * MAP_REF_TILE_SIZE };
                _tiles.emplace_back(std::make_unique<Tile>(gridPosition, position, _modelCache));
            }
        }

        _props.clear();
        createProps(mapDensity);
    }

}
