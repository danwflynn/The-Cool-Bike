#include "Engine.h"
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
        std::cout << "Failed to create window\n";
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    renderer.Init(window);

    Transform bikeTransform;
    bikeTransform.SetScale(glm::vec3(0.01f));
    bikeTransform.SetPosition(glm::vec3(0.0f, 0.0f, -2.0f));
    bikeTransform.SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));

    Camera camera(60.0f, 1280.0f / 720.0f, 0.1f, 100.0f);
    camera.SetPosition({ 0.0f, 0.5f, 5.0f });

    double lastX = 640, lastY = 360;
    bool firstMouse = true;

    float speed = 3.0f;
    float sensitivity = 0.15f;
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        renderer.BeginFrame();

        double time = glfwGetTime();
        float dt = float(time - lastTime);
        lastTime = time;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glm::vec3 pos = camera.GetPosition();
        glm::vec3 rot = camera.GetRotation();

        glm::vec3 forward = glm::normalize(glm::vec3(
            sin(rot.y), 0.0f, -cos(rot.y)
        ));
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0,1,0)));

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) pos += forward * speed * dt;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) pos -= forward * speed * dt;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) pos -= right * speed * dt;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) pos += right * speed * dt;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) pos.y += speed * dt;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) pos.y -= speed * dt;

        camera.SetPosition(pos);

        double x, y;
        glfwGetCursorPos(window, &x, &y);

        if (firstMouse) {
            lastX = x;
            lastY = y;
            firstMouse = false;
        }

        float dx = float(x - lastX) * sensitivity;
        float dy = float(y - lastY) * sensitivity;
        lastX = x;
        lastY = y;

        rot.y += glm::radians(dx);
        rot.x += glm::radians(dy);
        rot.x = glm::clamp(rot.x, glm::radians(-89.0f), glm::radians(89.0f));

        camera.SetRotation(rot);

        renderer.RenderBike(bikeTransform, camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}
