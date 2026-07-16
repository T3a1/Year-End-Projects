/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** ActionMap
*/

#include "ActionMap.hpp"
#include "Errors.hpp"

namespace Zappy {

void ActionMap::registerAction(const std::string &name, std::function<void()> action) {
    _actions[name] = std::move(action);
}

void ActionMap::trigger(const std::string &name) const {
    auto it = _actions.find(name);

    if (it == _actions.end())
        throw NotFoundError("Action not found: " + name);
    it->second();
}

}
