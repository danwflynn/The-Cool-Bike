#pragma once

#include "CameraController.h"
#include "controller/controller.h"
#include <GLFW/glfw3.h>

class FreeCameraController : public CameraController {
public:
    explicit FreeCameraController(Controller& controller);

    void SetWindow(GLFWwindow* window);

    void Update(Camera& camera, float dt) override;

private:
    void UpdateMouseLook(Camera& camera);

    GLFWwindow* window = nullptr;
    Controller& controller;

    float speed = 3.0f;
    float sensitivity = 0.15f;

    bool firstMouse = true;
    double lastX = 0.0, lastY = 0.0;
};
