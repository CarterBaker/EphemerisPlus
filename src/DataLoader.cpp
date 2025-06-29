#include "DataLoader.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

std::vector<SceneObject> DataLoader::LoadData(float KM_TO_OPENGL_UNIT) {
    std::vector<SceneObject> sceneObjects;

    for (const auto& entry : fs::directory_iterator(databaseDir)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".json")
            continue;

        std::ifstream file(entry.path());
        if (!file) {
            std::cerr << "Failed to open file: " << entry.path() << "\n";
            continue;
        }

        try {
            nlohmann::json j;
            file >> j;

            AstralData data = parseAstralBody(j);
            CameraData camData = j.contains("camera") ? parseCameraData(j["camera"]) : CameraData{};

            std::shared_ptr<Shader> shader = nullptr;
            if (!data.vertexShader.empty() && !data.fragmentShader.empty()) {
                shader = std::make_shared<Shader>(
                    shaderDir + data.vertexShader,
                    shaderDir + data.fragmentShader
                );
            }

            std::unique_ptr<Mesh> mesh = nullptr;
            if (!data.mesh.empty()) {
                mesh = std::make_unique<Mesh>();
                mesh->loadFromOBJ(resourceDir + data.mesh, resourceDir + data.texture);
            }

            std::shared_ptr<Mesh> iconQuad = nullptr;
            if (!data.icon.empty()) {
                iconQuad = std::make_shared<Mesh>();
                iconQuad->createQuad(resourceDir + data.icon);
            }

            glm::vec3 position(data.orbitRadius, 0.0f, 0.0f);

            SceneObject obj;
            obj.shader = shader;
            obj.mesh = std::move(mesh);
            obj.iconMesh = iconQuad;
            obj.position = position * KM_TO_OPENGL_UNIT;
            obj.scale = data.radius * KM_TO_OPENGL_UNIT;
            obj.magnificationScale = data.magnificationScale;
            obj.sourceData = data;
            obj.cameraData = camData;

            sceneObjects.push_back(std::move(obj));
        }
        catch (const std::exception& e) {
            std::cerr << "Error parsing JSON file " << entry.path() << ": " << e.what() << "\n";
        }
    }

    return sceneObjects;
}

AstralData DataLoader::parseAstralBody(const nlohmann::json& j) {
    AstralData data;
    data.name = j.value("name", "");
    data.mesh = j.value("mesh", "");
    data.texture = j.value("texture", "");
    data.icon = j.value("icon", "");
    data.vertexShader = j.value("vertexShader", "");
    data.fragmentShader = j.value("fragmentShader", "");
    data.radius = j.value("radius", 0.0f);
    data.magnificationScale = j.value("magnificationScale", 0.0f);
    data.rotationSpeed = j.value("rotationSpeed", 0.0f);
    data.orbitCenter = j.value("orbitCenter", "");
    data.orbitRadius = j.value("orbitRadius", 0.0f);
    data.tilt = j.value("tilt", 0.0f);
    return data;
}

CameraData DataLoader::parseCameraData(const nlohmann::json& j) {
    CameraData cam;
    cam.orbital = j.value("orbital", false);

    if (j.contains("focusPosition") && j["focusPosition"].is_array() && j["focusPosition"].size() == 3) {
        cam.focusPosition = glm::vec3(
            j["focusPosition"][0].get<float>(),
            j["focusPosition"][1].get<float>(),
            j["focusPosition"][2].get<float>()
        );
    }
    else {
        cam.focusPosition = glm::vec3(0.0f);
    }

    if (j.contains("cameraPosition") && j["cameraPosition"].is_array() && j["cameraPosition"].size() == 3) {
        cam.cameraPosition = glm::vec3(
            j["cameraPosition"][0].get<float>(),
            j["cameraPosition"][1].get<float>(),
            j["cameraPosition"][2].get<float>()
        );
    }
    else {
        cam.cameraPosition = glm::vec3(0.0f);
    }

    return cam;
}