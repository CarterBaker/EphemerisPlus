#include "HierarchyPanel.hpp"
#include "../src/AstralData.hpp"
#include <imgui/imgui.h>
#include <functional>
#include <iostream>
#include <unordered_set>

void HierarchyPanel::render(const std::vector<SceneObject>& sceneObjects) {
    // Map from AstralData.name to SceneObject pointer
    std::unordered_map<std::string, const SceneObject*> objectMap;
    // Children grouped by AstralData.orbitCenter (parent name)
    std::unordered_map<std::string, std::vector<const SceneObject*>> children;

    // Build lookup by sourceData.name
    for (const auto& obj : sceneObjects) {
        objectMap[obj.sourceData.name] = &obj;
    }

    // Build hierarchy: sourceData.orbitCenter -> children
    for (const auto& obj : sceneObjects) {
        const std::string& parent = obj.sourceData.orbitCenter;
        if (!parent.empty() && objectMap.count(parent)) {
            children[parent].push_back(&obj);
        }
    }

    std::function<void(const SceneObject*)> drawNode = [&](const SceneObject* obj) {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        if (selectedObject == obj->sourceData.name)
            flags |= ImGuiTreeNodeFlags_Selected;

        bool open = ImGui::TreeNodeEx((void*)obj, flags, "%s", obj->sourceData.name.c_str());

        if (ImGui::IsItemClicked()) {
            selectedObject = obj->sourceData.name;
            //std::cout << "[INFO] Selected: " << selectedObject << "\n";
            if (onSelect) onSelect(*obj);
        }

        if (open) {
            for (const SceneObject* child : children[obj->sourceData.name]) {
                drawNode(child);
            }
            ImGui::TreePop();
        }
    };

    ImGui::Begin("Hierarchy");

    for (const auto& obj : sceneObjects) {
        const std::string& parent = obj.sourceData.orbitCenter;
        if (parent.empty() || objectMap.find(parent) == objectMap.end()) {
            drawNode(&obj);
        }
    }

    ImGui::End();
}

