/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** ActionMap
*/

#pragma once
#include <functional>
#include <string>
#include <unordered_map>

namespace Zappy {
    class ActionMap {
        public:
            void registerAction(const std::string &name, std::function<void()> action);
            void trigger(const std::string &name) const;

        private:
            std::unordered_map<std::string, std::function<void()>> _actions;
    };
}
