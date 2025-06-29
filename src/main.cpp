#include <windows.h>
#include "../gui/viewport.hpp"
#include <iostream>

// Launch the main window.
int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    {
        Viewport viewport(1280, 720, "Ephemeris++");
        viewport.run();  // blocks until window closes
    }

    glfwTerminate();
    return 0;
}

// Windows GUI entry point required by linker.
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    return main();
}