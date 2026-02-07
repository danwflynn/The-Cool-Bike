#include "Engine.h"
#include "math/Camera.h"
#include "entities/Bike.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void Engine::RunWithFreeCamera()
{
    Run(CameraMode::Free);
}

void Engine::RunWithFollowCamera()
{
    Run(CameraMode::Follow);
}

void Engine::Run(CameraMode mode)
{
    if (!glfwInit())
        return;

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* modeInfo = glfwGetVideoMode(monitor);

    window = glfwCreateWindow(
        modeInfo->width,
        modeInfo->height,
        "My Engine",
        monitor,
        nullptr
    );

    if (!window) {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        return;
    }

    renderer.Init(window);

    Camera camera(
        60.0f,
        float(modeInfo->width) / float(modeInfo->height),
        0.1f,
        100.0f
    );
    camera.SetPosition({0.0f, 0.5f, 5.0f});

    Bike bike(camera);

    std::unique_ptr<CameraController> cameraController;

    if (mode == CameraMode::Free) {
        auto freeCam =
            std::make_unique<FreeCameraController>(controller);
        freeCam->SetWindow(window);
        cameraController = std::move(freeCam);
    }
    else {
        cameraController =
            std::make_unique<FollowCameraController>(bike);
    }

    double lastTime = glfwGetTime();

    std::cout << "Controller connected: "
              << controller.isConnected() << std::endl;
    std::cout << "Controller name: "
              << controller.getName() << std::endl;

    while (!glfwWindowShouldClose(window))
    {
        renderer.BeginFrame();

        double time = glfwGetTime();
        float dt = float(time - lastTime);
        lastTime = time;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        cameraController->Update(camera, dt);

        renderer.RenderBike(bike.getTransform(), camera);
        renderer.RenderGround(camera);

        glfwSwapBuffers(window);
        glfwPollEvents();

        bike.applyVerticalForce();
    }

    glfwTerminate();
}
