#pragma once

#include "Mesh.hpp"
#include "Shader.hpp"
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <string>
#include <memory>

struct AstralData {
    std::string name;
    std::string mesh;
    std::string texture;
    std::string icon;
    std::string vertexShader;
    std::string fragmentShader;
    float radius;
    float magnificationScale;
    glm::vec3 position;
    float rotationSpeed;
    std::string orbitCenter;
    float orbitRadius;
    float tilt;
};

struct CameraData {
    bool orbital;
    glm::vec3 focusPosition;
    glm::vec3 cameraPosition;
};

struct SceneObject {
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Mesh> iconMesh;
    glm::vec3 position;
    float scale;
    float magnificationScale;
    AstralData sourceData;
    CameraData cameraData;
};
