#include "Engine.h"
#include <iostream>

void Engine::Run() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        return;
    }

    window = glfwCreateWindow(1280, 720, "My Engine", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return;
    }

    renderer.Init();

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    while (!glfwWindowShouldClose(window)) {
        renderer.BeginFrame();

        // Later:
        // - update game logic
        // - submit draw calls to renderer

        renderer.EndFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    renderer.Shutdown();
    glfwTerminate();
}
