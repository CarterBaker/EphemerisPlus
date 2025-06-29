#include "viewport.hpp"
#include "../src/AstralData.hpp"
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <iostream>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "HierarchyPanel.hpp"

// Handles drawing the window and the tool bar at the top
Viewport::Viewport(int w, int h, const char* t)
    : width(w), height(h), title(t), window(nullptr),
    camera(0.0f, 0.0f, 0.0f)  // orbit radius, yaw, pitch in degrees
{
    init();
}

Viewport::~Viewport()
{
    cleanup();
}

void Viewport::init() {
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetScrollCallback(window, ScrollCallback);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        window = nullptr;
        return;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    hierarchyPanel.onSelect = [this](const SceneObject& obj) {
        this->HandleObjectSelection(obj);
    };

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    if (!universeRenderer) {
        universeRenderer = new UniverseRenderer();
    }

    universeRenderer->dataCallback = [this](const std::vector<SceneObject>& objs) {
        this->CacheSceneObjects(objs);
    };

    universeRenderer->init();
}

void Viewport::CacheSceneObjects(const std::vector<SceneObject>& sceneObjects) {
    //std::cout << "[DEBUG] caching: " << sceneObjects.size() << " objects\n";
    cachedSceneObjects = sceneObjects;
}

void Viewport::renderHierarchy(const std::vector<SceneObject>& sceneObjects) {
    hierarchyPanel.render(sceneObjects);
}

void Viewport::run() {
    if (!window) return;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        // --- Mouse input handling ---
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);

            if (!isDragging) {
                isDragging = true;
                lastMouseX = mouseX;
                lastMouseY = mouseY;
            }
            else {
                float deltaX = static_cast<float>(mouseX - lastMouseX);
                float deltaY = static_cast<float>(lastMouseY - mouseY); // invert Y for intuitive control

                camera.ProcessMouseMovement(deltaX, deltaY);

                lastMouseX = mouseX;
                lastMouseY = mouseY;
            }
        }
        else {
            isDragging = false;
        }

        camera.Update(); // Keep position in sync with object each frame

        // --- Render setup ---
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix((float)display_w / display_h);
        glm::vec3 position = camera.GetPosition();

        universeRenderer->render(view, projection, position);

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        renderHierarchy(cachedSceneObjects);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

}

void Viewport::HandleObjectSelection(const SceneObject& obj) {
    camera.FocusOnObject(obj);
}

void Viewport::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    // Retrieve the Viewport instance pointer
    Viewport* viewport = static_cast<Viewport*>(glfwGetWindowUserPointer(window));
    if (viewport) {
        // yoffset is vertical scroll amount
        viewport->camera.ProcessMouseScroll(static_cast<float>(yoffset));
    }
}

void Viewport::cleanup() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (window) glfwMakeContextCurrent(window);
    universeRenderer->cleanup();
    if (window) glfwDestroyWindow(window);

    window = nullptr;
}