#pragma once

#include "DataLoader.hpp"
#include "AstralData.hpp"
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Shader;
class Mesh;

class UniverseRenderer {
public:
    using DataCallback = std::function<void(const std::vector<SceneObject>&)>;
    DataCallback dataCallback;

    UniverseRenderer();
    ~UniverseRenderer();

    void init();
    void render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPosition);
    void cleanup();

    static constexpr float KM_TO_OPENGL_UNIT = 0.00001f;

private:
    void setupUniverse();
    void renderIcon(const SceneObject& obj, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos);

    DataLoader dataLoader;

    std::string baseVertDir = "shaders/base.vert";
    std::string baseFragDir = "shaders/base.frag";
    Shader* iconShader = nullptr;  // Shader used for rendering icons

    std::vector<AstralData> astralBodies;
    std::vector<SceneObject> sceneObjects;

    static constexpr float DISTANCE_THRESHOLD = 100.0f;
    static constexpr float nonlinearity = 0.5f;

};
