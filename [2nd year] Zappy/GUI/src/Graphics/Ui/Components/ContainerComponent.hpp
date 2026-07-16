/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** ContainerComponent
*/

#pragma once
#include "ASubWindowComponent.hpp"
#include <vector>
#include <memory>

namespace Zappy {
    class ContainerComponent : public ASubWindowComponent {
        public:
            ContainerComponent(float x = 0.5f, float y = 0.5f,
                float width = 1.f, float height = 1.f, int layer = 0);
            virtual ~ContainerComponent() = default;

            void update(float deltaTime) override;
            void draw(const Rectangle &parentRect) const override;
            bool onMouse(const Vector2&, bool) override;
            bool onMouse(const Vector2&, bool, const Rectangle &parentRect) override;
            void setVisible(bool visible) override;
            bool isVisible() const override;

            void clearChildren();
            void addChild(std::unique_ptr<ISubWindowComponent> child);
            void addSharedChild(std::shared_ptr<ISubWindowComponent> child);

        private:
            std::vector<std::unique_ptr<ISubWindowComponent>> _children;
            std::vector<std::shared_ptr<ISubWindowComponent>> _sharedChildren;
            bool _visible;
    };
}
