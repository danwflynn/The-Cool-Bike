#include "Engine.h"
#include "graphics/Shader.h"
#include "graphics/Mesh.h"
#include "math/Transform.h"
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

    uniform mat4 u_MVP;

    void main() {
        gl_Position = u_MVP * vec4(aPos, 1.0);
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
    Transform transform;

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        1280.0f / 720.0f,
        0.1f,
        100.0f
    );

    glm::mat4 view = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(0.0f, 0.0f, -2.0f)
    );

    while (!glfwWindowShouldClose(window)) {
        renderer.BeginFrame();

        glm::mat4 mvp = projection * view * transform.GetMatrix();
        shader.Bind();
        shader.SetMat4("u_MVP", mvp);
        triangleMesh.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}
