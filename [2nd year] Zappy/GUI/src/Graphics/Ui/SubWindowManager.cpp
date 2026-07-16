/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** SubWindowManager
*/

#include "SubWindowManager.hpp"
#include <algorithm>
#include "Errors.hpp"

namespace Zappy {

SubWindowManager::SubWindowManager()
{
}

SubWindow& SubWindowManager::addNormalizedSubWindow(const std::string &name,
    float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2,
    float animationSpeed, int layerIndex)
{
    auto window = std::make_unique<SubWindow>(
        SubWindow::createNormalized(
            x1, y1, w1, h1, x2, y2, w2, h2,
            animationSpeed, layerIndex
        )
    );

    _subwindows.push_back({ name, std::move(window) });
    return *_subwindows.back().window;
}

void SubWindowManager::closeAllSubWindows(const std::string &exception,
    const std::function<void(const std::string &name)> &onClose)
{
    for (auto &[name, window] : _subwindows) {
        if (name == exception || name == "main")
            continue;
        if (window->isVisible()) {
            if (onClose)
                onClose(name);
            window->togglePosition();
        }
    }
}
    
void SubWindowManager::clear() {
    _subwindows.clear();
}

void SubWindowManager::recomputeAllSubWindows() {
    for (auto &subwin : _subwindows)
        subwin.window->recomputeRects();
}

SubWindow& SubWindowManager::get(const std::string &name) {
    for (auto &subwindow : _subwindows)
        if (subwindow.name == name)
            return *subwindow.window;
    throw NotFoundError("SubWindow not found: " + name);
}

const SubWindow& SubWindowManager::get(const std::string &name) const {
    for (auto &subwindow : _subwindows)
        if (subwindow.name == name)
            return *subwindow.window;
    throw NotFoundError("SubWindow not found: " + name);
}

std::vector<std::reference_wrapper<SubWindow>> SubWindowManager::getAllActiveWindows() {
    std::vector<std::reference_wrapper<SubWindow>> result;

    for (auto &subwindow : _subwindows)
        if (subwindow.window)
            result.emplace_back(*subwindow.window);

    std::sort(result.begin(), result.end(),
        [](const std::reference_wrapper<SubWindow> &a,
        const std::reference_wrapper<SubWindow> &b) {
        return a.get().getLayerIndex() < b.get().getLayerIndex();
    });
    return result;
}

std::string SubWindowManager::getFirstWindowInLayerOrder(const Vector2 &mousePos) const {
    std::string firstWindowInLayerOrder = "";
    int highestLayer = -1;

    for (const auto &subwindow : _subwindows) {
        if (!subwindow.window->isVisible())
            continue;

        if (CheckCollisionPointRec(mousePos, subwindow.window->getRect())) {
            if (subwindow.window->getLayerIndex() > highestLayer) {
                highestLayer = subwindow.window->getLayerIndex();
                firstWindowInLayerOrder = subwindow.name;
            }
        }
    }
    return firstWindowInLayerOrder;
}

}
