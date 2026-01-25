#include "Engine.h"
#include "graphics/Shader.h"
#include "graphics/Mesh.h"
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

    static const char* vertexSrc = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    void main() {
        gl_Position = vec4(aPos, 1.0);
    }
    )";

    static const char* fragmentSrc = R"(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(1.0, 0.4, 0.2, 1.0);
    }
    )";

    float triangle[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    Shader shader(vertexSrc, fragmentSrc);
    Mesh triangleMesh(triangle, sizeof(triangle));

    while (!glfwWindowShouldClose(window)) {
        renderer.BeginFrame();

        shader.Bind();
        triangleMesh.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}
