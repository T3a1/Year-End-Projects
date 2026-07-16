/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Renderer
*/

#include "Renderer.hpp"
#include "Constants.hpp"
#include "SubWindow.hpp"
#include <cstdint>

namespace Zappy {

Renderer::Renderer() : _tileColor({60, 110, 60, 255}),
    _wireColor({80, 80, 80, 200}), _bgColor({18, 18, 24, 255})
{
}

void Renderer::drawGround(const Map &map) const {
    float tileSize = MAP_REF_TILE_SIZE;
    Vector3 center = {
        (map.getWidth() - 1) * tileSize / 2.0f,
        -0.1f,
        (map.getHeight() - 1) * tileSize / 2.0f
    };

    DrawModelEx(map.getGroundModel(), center, {0.f, 0.f, 0.f}, 0.f, { 1.f, 1.f, 1.f }, WHITE);
}

void Renderer::drawMap(const Map &map) const
{
    drawGround(map);
    const std::vector<std::unique_ptr<Tile>> &tiles = map.getTiles();
    
    for (const auto &tile : tiles) {

        Vector3 tilePos = tile->getPosition();

        const std::vector<std::unique_ptr<Food>> &foodItems = map.getCurrentTile(tilePos.x, tilePos.z)->getFood();

        for (const auto &food : foodItems) {
            DrawModelEx(food->getModel(), food->getPosition(),
                { 0.f, 1.f, 0.f }, food->getRotation().y,
                { food->getScale(), food->getScale(), food->getScale() }, WHITE);
        }

        const std::vector<std::unique_ptr<Stone>> &stones = map.getCurrentTile(tilePos.x, tilePos.z)->getStones();

        for (const auto &stone : stones) {
            DrawModelEx(stone->getModel(), stone->getPosition(),
                { 0.f, 1.f, 0.f }, stone->getRotation().y,
                { stone->getScale(), stone->getScale(), stone->getScale() }, WHITE);
        }
    }

    const std::vector<std::unique_ptr<Prop>> &props = map.getProps();
    
    for (const auto &prop : props) {
        DrawModelEx(prop->getModel(), prop->getPosition(),
            { 0.f, 1.f, 0.f }, prop->getRotation().y,
            { prop->getScale(), prop->getScale(), prop->getScale() }, WHITE);
    }
}

void Renderer::drawEntities(const std::vector<std::reference_wrapper<const IEntity>> &entities, const Raylib::ViewCamera &camera) const
{

    for (const auto &entity : entities) {
        if (!isVisible(entity.get().getPosition(), camera, MAX_VISIBILITY_DISTANCE))
            continue;

        float scale = entity.get().getScale();
        DrawModelEx(entity.get().getModel(), entity.get().getPosition(),
            { 0.f, 1.f, 0.f }, entity.get().getRotation().y,
            { scale, scale, scale }, entity.get().getColor());
    }
}

void Renderer::drawGrid(const Map &map) const {
    float tileSize = MAP_REF_TILE_SIZE;
    int width = map.getWidth();
    int height = map.getHeight();
    Color gridColor = DARKBROWN;
    float lineThickness = 0.05f;
    float lineHeight = 0.02f;
    float xCenter = (width - 1) * tileSize / 2.0f;
    float zCenter = (height - 1) * tileSize / 2.0f;

    for (int x = 0; x <= width; x++) {
        float xPos = x * tileSize - tileSize / 2.0f;

        Vector3 center = { xPos, lineHeight / 2.0f, zCenter };
        Vector3 size = { lineThickness, lineHeight, height * tileSize };
        DrawCube(center, size.x, size.y, size.z, gridColor);
    }

    for (int y = 0; y <= height; y++) {
        float zPos = y * tileSize - tileSize / 2.0f;

        Vector3 center = { xCenter, lineHeight / 2.0f, zPos };
        Vector3 size = { width * tileSize, lineHeight, lineThickness };
        DrawCube(center, size.x, size.y, size.z, gridColor);
    }
}

float Renderer::scaledFontSize(float baseFontSize) const {
    float ratioW = static_cast<float>(GetScreenWidth())  / 1920.f;
    float ratioH = static_cast<float>(GetScreenHeight()) / 1080.f;
    float ratio  = (ratioW + ratioH) * 0.5f;
    float scaled = baseFontSize * ratio;
    return scaled < 6.f ? 6.f : scaled;
}

void Renderer::drawSelectedTile(const Map &map, int tileX, int tileY) const {
    if (tileX < 0 || tileX >= map.getWidth() || tileY < 0 || tileY >= map.getHeight())
        return;

    float tileSize = MAP_REF_TILE_SIZE;
    float halfSize = tileSize * 0.5f;
    float yOffset = 0.02f;
    float lineThickness = 0.04f;
    int segmentCount  = 10;
    float segmentHeight = 0.08f;

    float centerX = (tileX + 0.5f) * tileSize - tileSize * 0.5f;
    float centerZ = (tileY + 0.5f) * tileSize - tileSize * 0.5f;
    std::vector<uint8_t> alphas(segmentCount);
    for (int i = 0; i < segmentCount; i++)
        alphas[i] = static_cast<uint8_t>(220 - i * (215 / (segmentCount - 1)));

    auto drawLineCube = [&](Vector3 start, Vector3 end, float thickness,
        float segY, float height, Color color) {
        Vector3 mid = { (start.x + end.x) * 0.5f, segY + height * 0.5f, (start.z + end.z) * 0.5f };

        Vector3 size = { 0.f, 0.f, 0.f };
        if (fabsf(end.x - start.x) > fabsf(end.z - start.z))
            size = { fabsf(end.x - start.x) + thickness, height, thickness };
        else
            size = { thickness, height, fabsf(end.z - start.z) + thickness };

        DrawCube(mid, size.x, size.y, size.z, color);
    };

    Vector3 topLeft = { centerX - halfSize, 0, centerZ - halfSize };
    Vector3 topRight = { centerX + halfSize, 0, centerZ - halfSize };
    Vector3 bottomLeft = { centerX - halfSize, 0, centerZ + halfSize };
    Vector3 bottomRight = { centerX + halfSize, 0, centerZ + halfSize };

    for (int segment = 0; segment < segmentCount; segment++) {
        float segmentY = yOffset + segment * segmentHeight;
        Color color = { 200, 180, 50, alphas[segment] };

        drawLineCube(topLeft, topRight, lineThickness, segmentY, segmentHeight, color);
        drawLineCube(bottomLeft, bottomRight, lineThickness, segmentY, segmentHeight, color);
        drawLineCube(topLeft, bottomLeft, lineThickness, segmentY, segmentHeight, color);
        drawLineCube(topRight, bottomRight, lineThickness, segmentY, segmentHeight, color);
    }
}

bool Renderer::isVisible(const Vector3 &position, const Raylib::ViewCamera &camera, float maxDistance) const {
    Vector3 toEntity = Vector3Subtract(position, camera.getCamera().position);
    float distance = Vector3Length(toEntity);

    if (distance > maxDistance)
        return false;
    if (distance < 5.0f)
        return true;

    Vector3 forward = Vector3Normalize(Vector3Subtract(camera.getCamera().target, camera.getCamera().position));
    Vector3 dir = Vector3Normalize(toEntity);
    float dot = Vector3DotProduct(forward, dir);

    return dot > 0.3f;
}

void Renderer::drawMenuImages(const std::map<std::string, Raylib::RectangleImage>& images, bool lockedFields) const {
    DrawTexturePro(images.at("Menu Background").getImageTexture().getTexture(),{ 0.f, 0.f,
          static_cast<float>(images.at("Menu Background").getImageTexture().getTexture().width),
          static_cast<float>(images.at("Menu Background").getImageTexture().getTexture().height) },
        images.at("Menu Background").getRectangle(),
        { 0.f, 0.f }, 0.f, WHITE
    );

    DrawRectangleRec(images.at("Main Panel").getRectangle(), images.at("Main Panel").getColor());
    DrawRectangleRec(images.at("Port Outline").getRectangle(), images.at("Port Outline").getColor());
    DrawRectangleRec(images.at("Machine Outline").getRectangle(), images.at("Machine Outline").getColor());
    DrawRectangleRec(images.at("Select Field Outline").getRectangle(), images.at("Select Field Outline").getColor());
    DrawRectangleRec(images.at("Port Background").getRectangle(), images.at("Port Background").getColor());
    DrawRectangleRec(images.at("Machine Background").getRectangle(), images.at("Machine Background").getColor());

    if (lockedFields) {
        DrawTexturePro(images.at("Port Lock").getImageTexture().getTexture(), { 0.f, 0.f, static_cast<float>(images.at("Port Lock").getImageTexture().getTexture().width), static_cast<float>(images.at("Port Lock").getImageTexture().getTexture().height) }, images.at("Port Lock").getRectangle(), { 0.f, 0.f }, 0.f, images.at("Port Lock").getColor());
        DrawTexturePro(images.at("Machine Lock").getImageTexture().getTexture(), { 0.f, 0.f, static_cast<float>(images.at("Machine Lock").getImageTexture().getTexture().width), static_cast<float>(images.at("Machine Lock").getImageTexture().getTexture().height) }, images.at("Machine Lock").getRectangle(), { 0.f, 0.f }, 0.f, images.at("Machine Lock").getColor());
    }
}

void Renderer::drawMenuTexts(const SubWindow& menuWindow, Raylib::TextFont& font,
    const std::string& port, const std::string& machine, bool correctValues) const
{
    int screenWidth = static_cast<int>(menuWindow.getRect().width);
    int screenHeight = static_cast<int>(menuWindow.getRect().height);

    auto drawCentered = [&](const std::string& text, Vector2 center, float fontSize, Color color) {
        Vector2 textSize = MeasureTextEx(font.getFont(), text.c_str(), fontSize, 0.f);
        Vector2 pos = { center.x - textSize.x * 0.5f, center.y - textSize.y * 0.5f };
        DrawTextEx(font.getFont(), text.c_str(), pos, fontSize, 0.f, color);
    };

    drawCentered("PORT", { 0.1f * screenWidth, 0.13f * screenHeight }, scaledFontSize(30.f), BROWN);
    drawCentered(port, { 0.1f * screenWidth, 0.2f  * screenHeight }, scaledFontSize(30.f), BROWN);
    drawCentered("MACHINE", { 0.1f * screenWidth, 0.28f * screenHeight }, scaledFontSize(30.f), BROWN);
    drawCentered(machine, { 0.1f * screenWidth, 0.35f * screenHeight }, scaledFontSize(25.f), BROWN);
    drawCentered("TGEgYnJhbmxlIGRlcyBzb3VyaXMK", { 0.1f * screenWidth, 0.05f * screenHeight }, scaledFontSize(25.f), BROWN);
    drawCentered("Start",{ 0.1f * screenWidth, 0.55f * screenHeight }, scaledFontSize(40.f), BROWN);
    drawCentered("Quit", { 0.1f * screenWidth, 0.7f  * screenHeight }, scaledFontSize(40.f), BROWN);

    if (!correctValues) {
        drawCentered("Impossible to connect...", { 0.1f * screenWidth, 0.8f  * screenHeight }, scaledFontSize(25.f), RED);
        drawCentered("Please check your values.", { 0.1f * screenWidth, 0.85f * screenHeight }, scaledFontSize(25.f), RED);
    }
}

void Renderer::drawMenuRats(const Zappy::Rat::ratInfo& rat, const std::vector<Zappy::Rat::ratPosition>& ratPositions,
    const SubWindow& menuWindow) const
{
    int totalFrames = rat.columns * rat.rows;
    int frame = static_cast<int>(GetTime() * 30) % totalFrames;

    Rectangle src = {
        static_cast<float>((frame % rat.columns) * rat.frameWidth),
        static_cast<float>((frame / rat.columns) * rat.frameHeight),
        static_cast<float>(rat.frameWidth),
        static_cast<float>(rat.frameHeight)
    };

    float screenW = static_cast<float>(menuWindow.getRect().width);
    float screenH = static_cast<float>(menuWindow.getRect().height);
    float sizeRatio = ((screenW / 1920.f) + (screenH / 1080.f)) * 0.5f;

    for (const auto& ratPos : ratPositions) {
        float scale = ratPos.scale * sizeRatio;
        float w = static_cast<float>(rat.frameWidth)  * scale;
        float h = static_cast<float>(rat.frameHeight) * scale;
        float x = ratPos.position.x * screenW - w * 0.5f;
        float y = ratPos.position.y * screenH - h * 0.5f;

        unsigned char shade = static_cast<unsigned char>(255 * ratPos.brightness);
        DrawTexturePro(rat.texture.getTexture(), src, { x, y, w, h }, { 0.f, 0.f }, 0.f,
            Color{ shade, shade, shade, 255 });
    }
}


}
