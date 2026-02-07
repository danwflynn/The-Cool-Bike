#include "FreeCameraController.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <iostream>

FreeCameraController::FreeCameraController(Controller& controller)
    : controller(controller) {
}

void FreeCameraController::SetWindow(GLFWwindow* window)
{
    this->window = window;
}

void FreeCameraController::Update(Camera& camera, float dt)
{
    if (!window)
        return;

    glm::vec3 pos = camera.GetPosition();
    glm::vec3 rot = camera.GetRotation();

    glm::vec3 forward = glm::normalize(glm::vec3(
        sin(rot.y),
        0.0f,
       -cos(rot.y)
    ));
    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) pos += forward * speed * dt;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) pos -= forward * speed * dt;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) pos -= right   * speed * dt;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) pos += right   * speed * dt;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)      pos.y += speed * dt;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) pos.y -= speed * dt;

    controller.update();

    float lx = controller.applyDeadzone(controller.leftStickX());
    float ly = controller.applyDeadzone(controller.leftStickY());

    pos -= forward * speed * dt * ly;
    pos += right   * speed * dt * lx;

    pos.y -= speed * dt * controller.applyDeadzone(controller.rightTrigger());
    pos.y += speed * dt * controller.applyDeadzone(controller.leftTrigger());

    camera.SetPosition(pos);

    UpdateMouseLook(camera);
}

void FreeCameraController::UpdateMouseLook(Camera& camera)
{
    if (!window)
        return;

    double x, y;
    glfwGetCursorPos(window, &x, &y);

    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
        return;
    }

    float dx = float(x - lastX) * sensitivity;
    float dy = float(y - lastY) * sensitivity;

    lastX = x;
    lastY = y;

    glm::vec3 rot = camera.GetRotation();

    rot.y += glm::radians(dx);
    rot.x += glm::radians(dy);

    rot.x = glm::clamp(rot.x, glm::radians(-89.0f), glm::radians(89.0f));

    camera.SetRotation(rot);
}
