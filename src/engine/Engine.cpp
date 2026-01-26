#include "Engine.h"
#include "graphics/Shader.h"
#include "graphics/Mesh.h"
#include "math/Transform.h"
#include "math/Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return;
    }

    renderer.Init();
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    static const char* vertexSrc = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aNormal;

        uniform mat4 u_MVP;
        uniform mat4 u_Model;

        out vec3 FragPos;
        out vec3 Normal;

        void main() {
            FragPos = vec3(u_Model * vec4(aPos, 1.0));
            Normal = mat3(transpose(inverse(u_Model))) * aNormal;
            gl_Position = u_MVP * vec4(aPos, 1.0);
        }
    )";

    static const char* fragmentSrc = R"(
        #version 330 core

        in vec3 FragPos;
        in vec3 Normal;

        out vec4 FragColor;

        uniform vec3 u_LightDir;
        uniform vec3 u_LightColor;
        uniform vec3 u_ObjectColor;
        uniform vec3 u_ViewPos;

        void main()
        {
            // Normalize inputs
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(-u_LightDir);

            // --- Ambient ---
            vec3 ambient = 0.35 * u_LightColor;

            // --- Diffuse ---
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * u_LightColor;

            // --- Specular ---
            vec3 viewDir = normalize(u_ViewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);

            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
            vec3 specular = 0.5 * spec * u_LightColor;

            vec3 result = (ambient + diffuse + specular) * u_ObjectColor;
            FragColor = vec4(result, 1.0);
        }
    )";

    Shader shader(vertexSrc, fragmentSrc);

    float cubeVertices[] = {
        // positions          // normals
        // Front (+Z)
        -0.5f,-0.5f, 0.5f,   0,0,1,
        0.5f,-0.5f, 0.5f,   0,0,1,
        0.5f, 0.5f, 0.5f,   0,0,1,
        -0.5f, 0.5f, 0.5f,   0,0,1,

        // Back (-Z)
        0.5f,-0.5f,-0.5f,   0,0,-1,
        -0.5f,-0.5f,-0.5f,   0,0,-1,
        -0.5f, 0.5f,-0.5f,   0,0,-1,
        0.5f, 0.5f,-0.5f,   0,0,-1,

        // Left (-X)
        -0.5f,-0.5f,-0.5f,  -1,0,0,
        -0.5f,-0.5f, 0.5f,  -1,0,0,
        -0.5f, 0.5f, 0.5f,  -1,0,0,
        -0.5f, 0.5f,-0.5f,  -1,0,0,

        // Right (+X)
        0.5f,-0.5f, 0.5f,   1,0,0,
        0.5f,-0.5f,-0.5f,   1,0,0,
        0.5f, 0.5f,-0.5f,   1,0,0,
        0.5f, 0.5f, 0.5f,   1,0,0,

        // Top (+Y)
        -0.5f, 0.5f, 0.5f,   0,1,0,
        0.5f, 0.5f, 0.5f,   0,1,0,
        0.5f, 0.5f,-0.5f,   0,1,0,
        -0.5f, 0.5f,-0.5f,   0,1,0,

        // Bottom (-Y)
        -0.5f,-0.5f,-0.5f,   0,-1,0,
        0.5f,-0.5f,-0.5f,   0,-1,0,
        0.5f,-0.5f, 0.5f,   0,-1,0,
        -0.5f,-0.5f, 0.5f,   0,-1,0
    };

    unsigned int cubeIndices[] = {
        0, 1, 2,  2, 3, 0,      // front
        4, 5, 6,  6, 7, 4,      // back
        8, 9,10, 10,11, 8,      // left
        12,13,14, 14,15,12,      // right
        16,17,18, 18,19,16,      // top
        20,21,22, 22,23,20       // bottom
    };

    Mesh cube(
        cubeVertices,
        sizeof(cubeVertices),
        cubeIndices,
        sizeof(cubeIndices),
        6 * sizeof(float)
    );
    Transform cubeTransform;

    Camera camera(60.0f, 1280.0f/720.0f, 0.1f, 100.0f);
    camera.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    camera.SetRotation(glm::vec3(0.0f));

    double lastX = 640, lastY = 360;
    bool firstMouse = true;

    float cameraSpeed = 3.0f;
    float mouseSensitivity = 0.15f;

    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        renderer.BeginFrame();

        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glm::vec3 camPos = camera.GetPosition();
        glm::vec3 rot = camera.GetRotation();

        glm::vec3 forward{
            sin(rot.y),
            0.0f,
            -cos(rot.y)
        };
        forward = glm::normalize(forward);

        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0,1,0)));
        glm::vec3 up = glm::vec3(0,1,0);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camPos += forward * cameraSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camPos -= forward * cameraSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camPos -= right * cameraSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camPos += right * cameraSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camPos += up * cameraSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camPos -= up * cameraSpeed * deltaTime;

        camera.SetPosition(camPos);

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = static_cast<float>(xpos - lastX);
        float yoffset = static_cast<float>(ypos - lastY);
        lastX = xpos;
        lastY = ypos;

        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        rot.y += glm::radians(xoffset);
        rot.x += glm::radians(yoffset);

        if (rot.x > glm::radians(89.0f)) rot.x = glm::radians(89.0f);
        if (rot.x < glm::radians(-89.0f)) rot.x = glm::radians(-89.0f);

        camera.SetRotation(rot);

        glm::mat4 mvp = camera.GetProjectionMatrix() * camera.GetViewMatrix() * cubeTransform.GetMatrix();
        shader.Bind();
        shader.SetVec3("u_ViewPos", camera.GetPosition());
        shader.SetVec3("u_LightDir", glm::vec3(-0.3f, -1.0f, -0.4f));
        shader.SetVec3("u_LightColor", glm::vec3(1.0f));
        shader.SetVec3("u_ObjectColor", glm::vec3(0.2f, 0.7f, 0.3f));
        shader.SetMat4("u_MVP", mvp);
        shader.SetMat4("u_Model", cubeTransform.GetMatrix());
        cube.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}
