/*
** EPITECH PROJECT, 2026
** GUI
** File description:
** Player
*/

#ifndef PLAYER_HPP_
#define PLAYER_HPP_
#include <array>
#include "AEntity.hpp"
#include <raylib.h>
#include "raymath.h"
#include <iostream>
#include "AnimationHandler.hpp"
#include "Map.hpp"
#include <array>
#include <queue>
#include <unordered_map>
#include <functional>

namespace Zappy {
    class Player : public AEntity {

        enum Direction {
            NORTH = 1,
            EAST = 2,
            SOUTH = 3,
            WEST = 4
        };

        public:
            enum MarkerShape { DIAMOND, DOWNARROW };

            Player(int id, std::string team, std::string modelPath, Vector2 SpawnTile, Vector3 rotation,
                float scale, Map& map, AnimationHandler& animationHandler, int timeUnit, ModelCache& modelCache);
            ~Player() override = default;

            void update(float deltaTime, bool shouldAnimate = true);
            void levelUp();
            void manageAnimations(float deltaTime);


            Vector2 getCurrentTile() const { return _currentTile; }
            int getLevel() const { return _level; }
            void setLevel(int level) { _level = level; }
            void setInventory(const std::array<int, RESOURCE_COUNT>& inventory) { _inventory = inventory; }
            std::array<int, RESOURCE_COUNT>& getInventory() { return _inventory; }
            const std::array<int, RESOURCE_COUNT>& getInventory() const { return _inventory; }
            int getId() const { return _id; }
            std::string getTeam() const { return _team; }
            const ModelAnimation* getAnims() const { return _anims; }
            int getAnimCount() const { return _animCount; }
            bool getIsLoaded() const { return _isLoaded; }
            int getCurrentAnim() const { return _currentAnim; }
            std::string getCurrentAnimation() const { return _currentAnimation; }
            bool isDead() const { return _isDead; }
            bool isIncanting() const { return _isIncanting; }
            Direction getDirection() const { return _orientation; }
   
            void setTargetTile(const Vector2& tile, int orientation);
            void setTimeUnit(int timeUnit);

            void startIncantation();
            void stopIncantation();

            void playAnimation(const std::string& name);

            bool isLoopingAnimation(const std::string& name);

            bool isDeathAnimationFinished();
            bool isFadingOut() const { return _isFading; };

            void setMarkerVisible(bool visible, float scale = 1.0f, MarkerShape shape = MarkerShape::DIAMOND);
            bool isMarkerVisible() const { return _markerVisible; }
            float getMarkerScale() const { return _markerScale; }
            Color getTeamColor() const { return _teamColor; }
            void setTeamColor(Color color) { _teamColor = color; }
            void setMarkerShape(MarkerShape shape) { _markerShape = shape; }
            void startEject(Direction orientation);
            void drawMarker();

        protected:
        private:
            Vector2 _targetTile;
            Vector2 _currentTile;
            int _id;

            Vector3 _startPosition  = {0, 0, 0};
            Vector3 _targetPosition = {0, 0, 0};
            float  _travelTime = 1.0f;
            float _elapsed = 0.0f;
            bool _isMoving = false;

            int _level = 1;

            AnimationHandler& _animationHandler;

            int _animCount = 0;
            ModelAnimation* _anims = nullptr;
            int _animFrame = 0;
            int _currentAnim = 0;
            float _animTime = 0.f;

            Direction _orientation;
            Map& _map;
            std::array<int, RESOURCE_COUNT> _inventory;
            std::string _team;

            int _timeUnit = 0;

            std::string _currentAnimation;

            std::array<std::string, 4> _loopingAnims = {"Run", "Idle", "Incantation", "Win"};

            float _deathTimer = 0.0f;
            bool _isFading = false;
            bool _isDead = false;

            bool _isIncanting = false;

            bool _hasWon = false;

            bool _isEjected = false;

            bool _markerVisible = false;
            float _markerScale = 1.0f;
            Color _teamColor = WHITE;
            MarkerShape _markerShape = MarkerShape::DIAMOND;
    };
};

#endif /* !PLAYER_HPP_ */