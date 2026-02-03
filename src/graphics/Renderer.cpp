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

    std::vector<MeshSplitRule> bikeSplits = {
        { "polygon2", SplitAxis::Z, 0.0f },
        { "polygon3", SplitAxis::Z, 0.0f }
    };
    m_Bike = std::make_unique<Model>(
        "assets/bike/standard_bike.glb",
        bikeSplits
    );

    m_GroundShader = std::make_unique<Shader>(
        "assets/shaders/ground_vert.glsl",
        "assets/shaders/ground_frag.glsl"
    );

    float vertices[] = {
        -500.0f, 0.0f, -500.0f,
        500.0f, 0.0f, -500.0f,
        500.0f, 0.0f,  500.0f,
        -500.0f, 0.0f,  500.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    m_groundMesh = std::make_unique<Mesh>(
        vertices,
        sizeof(vertices),
        indices,
        sizeof(indices),
        3 * sizeof(float)
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

    const auto& bike_meshes = m_Bike->getMeshes();
    for (int i = 0; i < bike_meshes.size(); i++) {
        if (i == 3 || i == 2) m_DefaultShader->SetVec3("u_ObjectColor", {0.7f, 0.2f, 0.3f});
        else m_DefaultShader->SetVec3("u_ObjectColor", {0.2f, 0.7f, 0.3f});
        bike_meshes[i].Draw();
    }

    m_DefaultShader->Unbind();
}

void Renderer::RenderGround(const Camera& camera) {
    m_GroundShader->Bind();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp =
        camera.GetProjectionMatrix() *
        camera.GetViewMatrix() *
        model;

    m_GroundShader->SetMat4("u_MVP", mvp);
    m_GroundShader->SetMat4("u_Model", model);

    m_GroundShader->SetFloat("u_TileSize", 1.0f);
    m_GroundShader->SetVec3("u_ColorA", {0.2f, 0.2f, 0.2f});
    m_GroundShader->SetVec3("u_ColorB", {0.8f, 0.8f, 0.8f});

    m_groundMesh->Draw();
    m_GroundShader->Unbind();
}


void Renderer::EndFrame() {
    // later: render queues, post-processing, UI
}
