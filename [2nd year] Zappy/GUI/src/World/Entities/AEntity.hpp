/*
** EPITECH PROJECT, 2026
** GUI
** File description:
** AEntity
*/

#ifndef AENTITY_HPP_
#define AENTITY_HPP_

#include <string>
#include "IEntity.hpp"
#include "raymath.h"
#include <iostream>
#include "ModelCache.hpp"

namespace Zappy {
    class AEntity : public IEntity {
        public:
            AEntity(std::string modelPath, Vector3 position, Vector3 rotation, float scale, ModelCache& modelCache, bool shared = true);
            ~AEntity() override;

            const Vector3& getPosition() const override { return _position; }
            const Vector3& getRotation() const override { return _rotation; }
            float getScale() const override { return _scale; }
            const Model& getModel() const override { return _model; }
            Model& getModel() override { return _model; }
            bool& isLoaded() override { return _isLoaded; }


            void setPosition(const Vector3 &position) override { _position = position; }
            void setRotation(const Vector3 &rotation) override { _rotation = rotation; }
            void setScale(const float scale) override { _scale = scale; }

            float getDistance(const IEntity& other) const override;
            float getDistance(const Vector3& point) const override;
            Color getColor() const override { return _alphaTint; }
            void setColor(const Color &color) override { _alphaTint = color; }


        protected:
        bool _isLoaded;
        
        private:
        
        Color _alphaTint = WHITE;
        Model _model;
        Vector3 _position;
        Vector3 _rotation;
        float _scale;
        ModelCache& _modelCache;

    };
};

#endif /* !AENTITY_HPP_ */
