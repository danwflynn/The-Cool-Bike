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

    // ----------------- Shader -----------------
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

        uniform vec3 u_LightDir = normalize(vec3(-1.0, -1.0, -1.0));
        uniform vec3 u_LightColor = vec3(1.0, 1.0, 1.0);
        uniform vec3 u_ObjectColor = vec3(0.2, 0.7, 0.3);

        void main() {
            vec3 ambient = 0.1 * u_LightColor;
            vec3 norm = normalize(Normal);
            float diff = max(dot(norm, -u_LightDir), 0.0);
            vec3 diffuse = diff * u_LightColor;
            vec3 result = (ambient + diffuse) * u_ObjectColor;
            FragColor = vec4(result, 1.0);
        }
    )";

    Shader shader(vertexSrc, fragmentSrc);

    // ----------------- Cube Mesh -----------------
    float cubeVertices[] = {
        // positions         // normals
        -0.5f,-0.5f, 0.5f, 0,0,1,
         0.5f,-0.5f, 0.5f, 0,0,1,
         0.5f, 0.5f, 0.5f, 0,0,1,
        -0.5f, 0.5f, 0.5f, 0,0,1,
        -0.5f,-0.5f,-0.5f, 0,0,-1,
         0.5f,-0.5f,-0.5f, 0,0,-1,
         0.5f, 0.5f,-0.5f, 0,0,-1,
        -0.5f, 0.5f,-0.5f, 0,0,-1
    };

    unsigned int cubeIndices[] = {
        0,1,2, 2,3,0,
        1,5,6, 6,2,1,
        5,4,7, 7,6,5,
        4,0,3, 3,7,4,
        3,2,6, 6,7,3,
        4,5,1, 1,0,4
    };

    Mesh cube(cubeVertices, sizeof(cubeVertices), cubeIndices, sizeof(cubeIndices));
    Transform cubeTransform;

    // ----------------- Camera -----------------
    Camera camera(60.0f, 1280.0f/720.0f, 0.1f, 100.0f);
    camera.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    camera.SetRotation(glm::vec3(0.0f)); // pitch, yaw, roll

    double lastX = 640, lastY = 360;
    bool firstMouse = true;

    float cameraSpeed = 3.0f;
    float mouseSensitivity = 0.15f;

    double lastTime = glfwGetTime();

    // ----------------- Main Loop -----------------
    while (!glfwWindowShouldClose(window)) {
        renderer.BeginFrame();

        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        // ----------------- Keyboard -----------------
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glm::vec3 camPos = camera.GetPosition();
        glm::vec3 rot = camera.GetRotation(); // pitch, yaw, roll

        // Forward (ignoring pitch)
        glm::vec3 forward{
            sin(rot.y),
            0.0f,
            -cos(rot.y) // negative here!
        };
        forward = glm::normalize(forward);

        // Right vector perpendicular to forward
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0,1,0)));
        glm::vec3 up = glm::vec3(0,1,0);

        // Keyboard movement
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camPos += forward * cameraSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camPos -= forward * cameraSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camPos -= right * cameraSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camPos += right * cameraSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camPos += up * cameraSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camPos -= up * cameraSpeed * deltaTime;

        camera.SetPosition(camPos);

        // ----------------- Mouse Look -----------------
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = static_cast<float>(xpos - lastX);
        float yoffset = static_cast<float>(ypos - lastY); // y-axis screen coordinates
        lastX = xpos;
        lastY = ypos;

        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        rot.y += glm::radians(xoffset);  // yaw
        rot.x += glm::radians(yoffset);  // pitch inverted correctly

        if (rot.x > glm::radians(89.0f)) rot.x = glm::radians(89.0f);
        if (rot.x < glm::radians(-89.0f)) rot.x = glm::radians(-89.0f);

        camera.SetRotation(rot);

        // ----------------- Draw Cube -----------------
        glm::mat4 mvp = camera.GetProjectionMatrix() * camera.GetViewMatrix() * cubeTransform.GetMatrix();
        shader.Bind();
        shader.SetMat4("u_MVP", mvp);
        shader.SetMat4("u_Model", cubeTransform.GetMatrix());
        cube.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}
