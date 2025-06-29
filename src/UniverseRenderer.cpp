#include "UniverseRenderer.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Icon.hpp"
#include "AstralData.hpp"

#include <iostream>

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

UniverseRenderer::UniverseRenderer() {}

UniverseRenderer::~UniverseRenderer() {
    cleanup();
}

void UniverseRenderer::init() {
    cleanup();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    iconShader = new Shader(baseVertDir.c_str(), baseFragDir.c_str());

    setupUniverse();
}

void UniverseRenderer::render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPosition) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::vector<const SceneObject*> opaqueObjects;
    std::vector<const SceneObject*> transparentObjects;

    for (const auto& obj : sceneObjects) {
        float distance = glm::length(obj.position - cameraPosition);

        if (!obj.mesh || !obj.shader || distance > DISTANCE_THRESHOLD) {
            transparentObjects.push_back(&obj);
        }
        else {
            opaqueObjects.push_back(&obj);
        }
    }

    // Render opaque objects normally
    for (const auto* obj : opaqueObjects) {
        obj->shader->use();

        glm::mat4 model = glm::translate(glm::mat4(1.0f), obj->position);
        model = glm::scale(model, glm::vec3(obj->scale * 2.0f));

        obj->shader->setMat4("model", model);
        obj->shader->setMat4("view", view);
        obj->shader->setMat4("projection", projection);

        obj->mesh->draw();
    }

    // Sort transparent objects back-to-front
    std::sort(transparentObjects.begin(), transparentObjects.end(),
        [&cameraPosition](const SceneObject* a, const SceneObject* b) {
            float da = glm::length(a->position - cameraPosition);
            float db = glm::length(b->position - cameraPosition);
            return da > db; // farther objects first
        });

    // Render icons (transparent objects)
    for (const auto* obj : transparentObjects) {
        renderIcon(*obj, view, projection, cameraPosition);
    }
}


void UniverseRenderer::renderIcon(const SceneObject& obj, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos) {
    if (!obj.iconMesh) return;

    iconShader->use();

    float distance = glm::length(obj.position - cameraPos);

    // Base scale at distance threshold
    float sizeAtThreshold = obj.scale * 2.0f;

    // If beyond threshold, scale quad size proportional to distance
    float quadScale = sizeAtThreshold * obj.magnificationScale * powf(distance / DISTANCE_THRESHOLD, nonlinearity);

    glm::vec3 objPos = obj.position; // Object position
    glm::vec3 lookDir = glm::normalize(cameraPos - objPos); // Vector from camera to object (billboard look direction)
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // Camera's up vector
    glm::vec3 right = glm::normalize(glm::cross(up, lookDir)); // Right vector perpendicular to up and lookDir
    up = glm::cross(lookDir, right);
    glm::mat4 model(1.0f);

    // Set right, up scaled by quadScale, no forward scale on billboard
    model[0] = glm::vec4(right * quadScale, 0.0f);
    model[1] = glm::vec4(up * quadScale, 0.0f);
    model[2] = glm::vec4(lookDir * 0.0f, 0.0f);
    model[3] = glm::vec4(objPos, 1.0f);

    iconShader->setMat4("model", model);
    iconShader->setMat4("view", view);
    iconShader->setMat4("projection", projection);

    // Bind the icon texture
    glActiveTexture(GL_TEXTURE0);
    iconShader->setInt("texture1", 0); // Assuming uniform name 'texture1' in shader

    glDepthMask(GL_FALSE);
    obj.iconMesh->draw();
    glDepthMask(GL_TRUE);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void UniverseRenderer::setupUniverse() {
    sceneObjects.clear();

    sceneObjects = dataLoader.LoadData(KM_TO_OPENGL_UNIT);

    if (dataCallback) {
        dataCallback(sceneObjects);
    }
}


void UniverseRenderer::cleanup() {
    sceneObjects.clear();

    if (iconShader) {
        delete iconShader;
        iconShader = nullptr;
    }
}
