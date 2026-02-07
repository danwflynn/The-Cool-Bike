#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "graphics/Renderer.h"
#include "controller/controller.h"
#include <memory>
#include "math/camera-control/FreeCameraController.h"
#include "math/camera-control/FollowCameraController.h"

class Engine {
public:
    void RunWithFreeCamera();
    void RunWithFollowCamera();

private:
    enum class CameraMode {
        Free,
        Follow
    };

    void Run(CameraMode mode);

private:
    GLFWwindow* window = nullptr;
    Renderer renderer;
    Controller controller;
};
