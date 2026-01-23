#include "Engine.h"
#include <GLFW/glfw3.h>

void Engine::Run() {
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(1280, 720, "My Engine", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}
