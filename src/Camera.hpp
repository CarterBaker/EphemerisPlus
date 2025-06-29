// Camera.hpp
#include "AstralData.hpp"
#include <optional>

class Camera {
public:
    Camera(float radius, float yawDeg, float pitchDeg);

    glm::vec3 GetPosition() const;
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix(float aspectRatio) const;

    void FocusOnObject(const SceneObject& obj);
    void Update(); // Call this per frame
    void ProcessMouseMovement(float deltaX, float deltaY);
    void ProcessMouseScroll(float deltaScroll);

private:
    float zoom = 4.0f;
    float radius;
    float yaw;
    float pitch;

    glm::vec3 target;
    glm::vec3 position;

    std::optional<SceneObject> focusedObject;  // Stored for updates
};
