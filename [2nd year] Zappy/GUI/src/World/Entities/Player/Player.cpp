/*
** EPITECH PROJECT, 2026
** GUI
** File description:
** Player
*/

#include "Player.hpp"

namespace Zappy {

    Player::Player(int id, std::string team, std::string modelPath, Vector2 SpawnTile, Vector3 rotation,
        float scale, Map& map, AnimationHandler& animationHandler, int timeUnit, ModelCache& modelCache)
        : AEntity(modelPath, {
            map.getCurrentTile(static_cast<int>(SpawnTile.x), static_cast<int>(SpawnTile.y))->getWorldPosition().x,
            0.f,
            map.getCurrentTile(static_cast<int>(SpawnTile.x), static_cast<int>(SpawnTile.y))->getWorldPosition().z
        }, rotation, scale, modelCache, false), _id(id), _animationHandler(animationHandler), _map(map), _team(team), _timeUnit(timeUnit)
    {
        _currentTile = SpawnTile;
        _inventory.fill(0);

        playAnimation("Stand Up");

        _currentAnimation = "Stand Up";

        _animationHandler.changeTexture(getModel(), "Level 1");

    }

    void Player::setTargetTile(const Vector2& target, int orientation)
    {
        if (_isDead || _hasWon)
            return;

        if (_isEjected)
            _isEjected = false;
        else 
            _orientation = static_cast<Direction>(orientation);


        Vector3 targetWorldPos = _map.getCurrentTile(static_cast<int>(target.x), static_cast<int>(target.y))->getWorldPosition();

        Vector3 start = getPosition();

        int width = _map.getWidth();
        int height = _map.getHeight();
        int dx = static_cast<int>(target.x) - static_cast<int>(_currentTile.x);
        int dy = static_cast<int>(target.y) - static_cast<int>(_currentTile.y);

        if (std::abs(dx) > width / 2)
            start.x = (dx > 0) ? targetWorldPos.x + MAP_REF_TILE_SIZE : targetWorldPos.x - MAP_REF_TILE_SIZE;

        if (std::abs(dy) > height / 2)
            start.z = (dy > 0) ? targetWorldPos.z + MAP_REF_TILE_SIZE : targetWorldPos.z - MAP_REF_TILE_SIZE;

        _startPosition = start;
        _targetPosition = targetWorldPos;
        _travelTime = (_timeUnit > 0) ? 7.0f / static_cast<float>(_timeUnit) : 1.0f;
        _elapsed = 0.0f;
        _isMoving = true;
        _targetTile = target;

        switch (_orientation) {
            case Direction::NORTH:
                setRotation({0, 180, 0}); 
                break;
            case Direction::SOUTH:
                setRotation({0,   0, 0});
                break;
            case Direction::EAST:
                setRotation({0,  90, 0});
                break;
            case Direction::WEST:
                setRotation({0, 270, 0}); break;
        }
    }

    void Player::manageAnimations(float deltaTime)
    {
        if (_animCount <= 0 || _anims == nullptr)
            return;

        int frameCount = _anims[_currentAnim].frameCount;

        bool isLooping = false;
        for (auto anim : _loopingAnims) {
            if (_currentAnimation == anim) {
                isLooping = true;
                break;
            }
        }

        float speed;
        if (isLooping || _timeUnit <= 0) {
            speed = PLAYER_BASE_ANIMATION_SPEED;
        } else {
            float durationSeconds = 7.0f / static_cast<float>(_timeUnit);
            speed = static_cast<float>(frameCount) / durationSeconds;
        }

        if (_currentAnimation == "Die") {
            _animTime = std::min(_animTime + deltaTime * speed, static_cast<float>(frameCount - 1));
            _animFrame = static_cast<int>(_animTime);
            UpdateModelAnimation(getModel(), _anims[_currentAnim], _animFrame);

            if (_animTime >= static_cast<float>(frameCount - 1)) {
                if (!_isFading) {
                    _deathTimer += deltaTime;
                    if (_deathTimer >= 3.0f)
                        _isFading = true;
                } else {
                    int alpha = static_cast<int>(getColor().a);

                    alpha -= static_cast<int>((255.0f / 1.5f) * deltaTime);
                    if (alpha < 0)
                        alpha = 0;

                    Color color = getColor();
                    color.a = static_cast<unsigned char>(alpha);
                    setColor(color);
                }
            }
            return;
        }

        _animTime += deltaTime * speed;

        if (!isLooping && _animTime >= static_cast<float>(frameCount)) {
            playAnimation("Idle");;
            frameCount = _anims[_currentAnim].frameCount;
        }

        _animFrame = static_cast<int>(_animTime) % frameCount;
        UpdateModelAnimation(getModel(), _anims[_currentAnim], _animFrame);
    }

    void Player::levelUp()
    {
        if (_level >= 8) {
            _level = 8;
            return;
        }
        _level++;
        std::string textureName = "Level " + std::to_string(_level);
        _animationHandler.changeTexture(getModel(), textureName);
       setScale(getScale() + 0.0008f);
    }

    void Player::update(float deltaTime, bool shouldAnimate)
    {
        if (shouldAnimate)
            manageAnimations(deltaTime);

        if (_hasWon || _isDead) {
            return;
        }

        if (_isMoving) {
            _elapsed += deltaTime;

            float ratio = _elapsed / _travelTime;
            if (ratio >= 1.0f) {
                ratio = 1.0f;
                _isMoving = false;
                _currentTile = _targetTile;
            }

            setPosition(Vector3Lerp(_startPosition, _targetPosition, ratio));

            if (!_isEjected && _currentAnimation != "Run")
                playAnimation("Run");
        } else {
            if (_currentAnimation == "Run")
                playAnimation("Idle");
        }
    }

    void Player::startIncantation()
    {
        _isIncanting = true;
        playAnimation("Incantation");
    }

    void Player::stopIncantation()
    {
        _isIncanting = false;
        if (_currentAnimation == "Incantation")
            playAnimation("Idle");
    }

    void Player::playAnimation(const std::string& name)
    {
        if (_hasWon && name != "Win")
            return;

        if (_currentAnimation == name || _isDead)
            return;

        if (name == "Die") {
            _isDead = true;
            _isFading = false;
            _deathTimer = 0.0f;
            setColor(WHITE);
        }

        _animationHandler.playAnimation(name, _anims, _animCount, isLoaded(), _currentAnim, _animFrame, _animTime);
        _currentAnimation = name;

        if (name == "Win")
            _hasWon = true;
    }

    bool Player::isLoopingAnimation(const std::string& name)
    {
        for (const auto& anim : _loopingAnims) {
            if (anim == name)
                return true;
        }
        return false;
    }

    bool Player::isDeathAnimationFinished()
    {
        if (_currentAnimation != "Die" || _animCount <= 0 || _anims == nullptr)
            return false;

        return _isFading && getColor().a <= 0.1f;
    }

    void Player::setMarkerVisible(bool visible, float scale, MarkerShape shape) {
        _markerVisible = visible;
        _markerScale = scale;
        _markerShape = shape;
    }

    void Player::drawMarker() {
        if (!_markerVisible)
            return;

        Vector3 pos = getPosition();
        float heightOffset = 3.0f + (getScale() - 0.01f) / 0.0008f * 0.3f;
        pos.y += heightOffset;
        float size = 0.5f * _markerScale;

        static const std::unordered_map<MarkerShape, std::function<void(const Vector3&, float, Color)>> drawFunctions = {
            {MarkerShape::DIAMOND, [](const Vector3& pos, float size, Color color) {
                Vector3 upperPos = pos + Vector3{0, size / 4, 0};
                DrawCylinder(upperPos, 0.0f, size / 2, size / 2, 8, color);

                Vector3 lowerPos = pos - Vector3{0, size / 4, 0};
                DrawCylinder(lowerPos, size / 2, 0.0f, size / 2, 8, color);
            }},

            {MarkerShape::DOWNARROW, [](const Vector3& pos, float size, Color color) {
                Vector3 bodyPos = pos + Vector3{0, size / 4, 0};
                DrawCylinder(bodyPos, size / 6, size / 6, size / 2, 8, color);

                Vector3 pointingPos = pos - Vector3{0, size / 4, 0};
                DrawCylinder(pointingPos, size / 3, 0.0f, size / 2, 8, color);
            }}
        };

        auto it = drawFunctions.find(_markerShape);
        if (it != drawFunctions.end())
            it->second(pos, size, _teamColor);
    }

    void Player::startEject(Direction orientation) {
        _orientation = orientation;
        _isEjected = true;

        switch (orientation) {
            case Direction::NORTH:
                setRotation({0, 180, 0}); break;
            case Direction::SOUTH:
                setRotation({0,   0, 0}); break;
            case Direction::EAST:
                setRotation({0,  90, 0}); break;
            case Direction::WEST:
                setRotation({0, 270, 0}); break;
        }
    }

    void Player::setTimeUnit(int timeUnit) {
        _timeUnit = timeUnit;
    }

}
