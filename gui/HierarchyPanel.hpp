#pragma once

#include "../src/AstralData.hpp"
#include <vector>
#include <unordered_map>
#include <string>

class HierarchyPanel {
public:
    void render(const std::vector<SceneObject>& sceneObjects);
    std::string selectedObject; // Name of currently selected object

    std::function<void(const SceneObject&)> onSelect;
};
