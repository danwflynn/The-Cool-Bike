#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Model.h"
#include "../math/Transform.h"
#include "../math/Camera.h"

class Renderer {
public:
    void Init(GLFWwindow* window);
    void BeginFrame();
    void RenderBike(const Transform& transform, const Camera& camera);
    void RenderGround(const Camera& camera);
    void EndFrame();

    float getBikeBottomY();

private:
    std::unique_ptr<Shader> m_DefaultShader;
    std::unique_ptr<Shader> m_GroundShader;
    std::unique_ptr<Model> m_Bike;
    std::unique_ptr<Mesh> m_groundMesh;
};
