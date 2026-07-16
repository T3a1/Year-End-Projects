/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** Stone
*/

#ifndef STONE_HPP_
#define STONE_HPP_
#include "AEntity.hpp"

namespace Zappy {
    enum class StoneType {
        LINEMATE,
        DERAUMERE,
        SIBUR,
        MENDIANE,
        PHIRAS,
        THYSTAME
    };

    class Stone : public AEntity {
        public:
            Stone(std::string modelPath, Vector3 position, Vector3 rotation, float scale,
                StoneType type, ModelCache& modelCache);
            ~Stone();

            StoneType getType() const { return _type; }
            float getBobPhase() const { return _bobPhase; }

        protected:
        private:
            StoneType _type;
            float _bobPhase;
    };
}

#endif /* !STONE_HPP_ */
