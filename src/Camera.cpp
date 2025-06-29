#include "Camera.hpp"
#include "AstralData.hpp"
#include <algorithm>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../src/UniverseRenderer.hpp"

Camera::Camera(float radius, float yawDeg, float pitchDeg)
    : radius(radius), yaw(glm::radians(yawDeg)), pitch(glm::radians(pitchDeg)), target(0.0f) {}

glm::vec3 Camera::GetPosition() const {
    if (focusedObject.has_value() && !focusedObject->cameraData.orbital) {
        return position; // Use manually-set absolute position
    }

    // Orbital: Convert spherical to cartesian
    float x = radius * cos(pitch) * cos(yaw);
    float y = radius * sin(pitch);
    float z = radius * cos(pitch) * sin(yaw);
    return target + glm::vec3(x, y, z);
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(GetPosition(), target, glm::vec3(0, 1, 0));
}

glm::mat4 Camera::GetProjectionMatrix(float aspectRatio) const {
    return glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1e13f);
}

void Camera::FocusOnObject(const SceneObject& obj) {
    focusedObject = obj;

    if (!obj.cameraData.orbital) {
        // Non-orbital: fixed position and target, no zoom or mouse control
        position = obj.cameraData.cameraPosition;
        target = obj.cameraData.focusPosition;
        radius = glm::length(position - target);
        zoom = 1.0f;
    }
    else {
        // Orbital: use the object's position as target and derive position
        target = obj.position;
        radius = obj.sourceData.radius * UniverseRenderer::KM_TO_OPENGL_UNIT * zoom;
        position = GetPosition(); // Based on spherical coords
        zoom = 6.0f;
    }
}

void Camera::Update() {
    if (!focusedObject.has_value()) return;

    if (focusedObject->cameraData.orbital) {
        target = focusedObject->position;
        position = GetPosition(); // Based on yaw/pitch and radius from target
    }
}

void Camera::ProcessMouseMovement(float deltaX, float deltaY) {

    // Ignore scroll input if non-orbital mode
    if (!focusedObject.has_value()) return;
    if (!focusedObject->cameraData.orbital) return;

    const float sensitivity = 0.005f;
    yaw += deltaX * sensitivity;
    pitch += deltaY * sensitivity;

    // Clamp pitch to avoid flipping
    float pitchLimit = glm::half_pi<float>() - 0.01f;
    pitch = std::clamp(pitch, -pitchLimit, pitchLimit);

    Update(); // Update camera position
}

void Camera::ProcessMouseScroll(float deltaScroll) {

    // Ignore scroll input if non-orbital mode
    if (!focusedObject.has_value()) return;
    if (!focusedObject->cameraData.orbital) return;

    // Adjust zoom based on scroll input
    float zoomSensitivity = 0.1f;
    zoom -= deltaScroll * zoomSensitivity;

    // Clamp zoom to avoid extreme zooming
    zoom = std::clamp(zoom, 0.1f, 20.0f);

    if (focusedObject.has_value() && focusedObject->cameraData.orbital) {
        radius = focusedObject->sourceData.radius * UniverseRenderer::KM_TO_OPENGL_UNIT * zoom;
        Update();
    }
}
