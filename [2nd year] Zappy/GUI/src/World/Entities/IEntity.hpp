/*
** EPITECH PROJECT, 2026
** GUI
** File description:
** IEntity
*/

#include <string>
#include <raylib.h>

#ifndef IENTITY_HPP_
#define IENTITY_HPP_

namespace Zappy {
    class IEntity {
        public:
            virtual ~IEntity() = default;

            virtual const Vector3& getPosition() const = 0;
            virtual const Vector3& getRotation() const = 0;
            virtual float getScale() const = 0;
            virtual const Model& getModel() const = 0;
            virtual Model& getModel() = 0;
            virtual float getDistance(const IEntity& other) const = 0;
            virtual float getDistance(const Vector3& point) const = 0;
            virtual bool& isLoaded() = 0;
            virtual Color getColor() const = 0;

            virtual void setPosition(const Vector3 &position) = 0;
            virtual void setRotation(const Vector3 &rotation) = 0;
            virtual void setScale(const float scale) = 0;
            virtual void setColor(const Color &color) = 0;
            
    };
};

#endif /* !IENTITY_HPP_ */
