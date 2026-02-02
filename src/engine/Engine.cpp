#include "Engine.h"
#include "math/Camera.h"
#include "entities/Bike.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


void Engine::Run() {
    if (!glfwInit()) return;

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    window = glfwCreateWindow(mode->width, mode->height, "My Engine", monitor, nullptr);
    if (!window) {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    renderer.Init(window);

    Camera camera(60.0f, float(mode->width) / float(mode->height), 0.1f, 100.0f);
    camera.SetPosition({0.0f, 0.5f, 5.0f});

    Bike bike(camera);

    double lastX = 640, lastY = 360;
    bool firstMouse = true;

    float speed = 3.0f;
    float sensitivity = 0.15f;
    double lastTime = glfwGetTime();

    std::cout << "Connected: " << controller.isConnected() << std::endl;
    std::cout << "Controller name: " << controller.getName() << std::endl;

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

        controller.update();
        pos -= forward * speed * dt * controller.applyDeadzone(controller.leftStickY());
        pos += right * speed * dt * controller.applyDeadzone(controller.leftStickX());
        pos.y -= speed * dt * controller.applyDeadzone(controller.rightTrigger());
        pos.y += speed * dt * controller.applyDeadzone(controller.leftTrigger());

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

        renderer.RenderBike(bike.getTransform(), camera);
        renderer.RenderGround(camera);

        glfwSwapBuffers(window);
        glfwPollEvents();

        glm::vec3 bikePos = bike.getTransform().GetPosition();
        if (!bike.IsCollidingWithGround()) bike.getTransform().SetPosition(glm::vec3(bikePos.x, bikePos.y - 0.001, bikePos.z));
    }

    glfwTerminate();
}
