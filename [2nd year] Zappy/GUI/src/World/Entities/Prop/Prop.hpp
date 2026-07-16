/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** Prop
*/

#ifndef PROP_HPP_
#define PROP_HPP_

#include "AEntity.hpp"

namespace Zappy {
    class Prop : public AEntity {
        public:
            Prop(std::string modelPath, Vector3 position, Vector3 rotation,
                float scale, ModelCache& modelCache);
            ~Prop();

        protected:
        private:
    };
}

#endif /* !PROP_HPP_ */
