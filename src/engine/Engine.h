#pragma once
#include "graphics/Renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "controller/controller.h"

class Engine {
public:
    void Run();
private:
    GLFWwindow* window;
    Renderer renderer;
    Controller controller;
};
