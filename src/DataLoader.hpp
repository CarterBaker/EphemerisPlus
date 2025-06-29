#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "AstralData.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"

class DataLoader {
public:
    // Public method to load and assemble scene objects from JSON files
    std::vector<SceneObject> LoadData(float KM_TO_OPENGL_UNIT);

private:
    AstralData parseAstralBody(const nlohmann::json& j);
    CameraData parseCameraData(const nlohmann::json& j);

    std::string databaseDir = "database/";
    std::string resourceDir = "resources/";
    std::string shaderDir = "shaders/";

    std::string baseVertDir = "shaders/base.vert";
    std::string baseFragDir = "shaders/base.frag";

    // Optionally you could have private helper methods for shader/mesh loading
    // Shader* loadShader(const std::string& vertexPath, const std::string& fragmentPath);
    // Mesh* loadMesh(const std::string& meshPath, const std::string& texturePath);
};
