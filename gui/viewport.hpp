#include "../src/UniverseRenderer.hpp"
#include "../src/AstralData.hpp"
#include <GLFW/glfw3.h>
#include "../src/Camera.hpp" 
#include "HierarchyPanel.hpp"

class Viewport {
public:
    Viewport(int w, int h, const char* t);
    ~Viewport();

    void run();

    void CacheSceneObjects(const std::vector<SceneObject>& sceneObjects);

private:
    void init();
    void cleanup();

    void updateCamera();

    void renderHierarchy(const std::vector<SceneObject>& sceneObjects);
    void HandleObjectSelection(const SceneObject& obj);

    int width, height;
    const char* title;
    GLFWwindow* window;

    HierarchyPanel hierarchyPanel;

    Camera camera;

    UniverseRenderer* universeRenderer;
    std::vector<SceneObject> cachedSceneObjects;

    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
    bool isDragging = false;

    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};
