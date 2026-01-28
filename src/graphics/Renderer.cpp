#include "Renderer.h"
#include <iostream>

void Renderer::Init(GLFWwindow* window) {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    m_DefaultShader = std::make_unique<Shader>(
        "assets/shaders/default_vert.glsl",
        "assets/shaders/default_frag.glsl"
    );

    m_Bike = std::make_unique<Model>(
        "assets/bike/standard_bike.glb"
    );
}

void Renderer::BeginFrame() {
    glClearColor(0.1f, 0.2f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::RenderBike(const Transform& transform, const Camera& camera) {
    m_DefaultShader->Bind();

    m_DefaultShader->SetVec3("u_ViewPos", camera.GetPosition());
    m_DefaultShader->SetVec3("u_LightDir", glm::vec3(-0.3f, -1.0f, -0.4f));
    m_DefaultShader->SetVec3("u_LightColor", glm::vec3(1.0f));
    m_DefaultShader->SetVec3("u_ObjectColor", {0.2f, 0.7f, 0.3f});

    glm::mat4 mvp =
        camera.GetProjectionMatrix() *
        camera.GetViewMatrix() *
        transform.GetMatrix();

    m_DefaultShader->SetMat4("u_MVP", mvp);
    m_DefaultShader->SetMat4("u_Model", transform.GetMatrix());

    m_Bike->Draw(*m_DefaultShader);
}

void Renderer::EndFrame() {
    // later: render queues, post-processing, UI
}
