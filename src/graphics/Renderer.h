#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Renderer {
public:
    void Init(GLFWwindow* window);
    void BeginFrame();
    void EndFrame();
};
