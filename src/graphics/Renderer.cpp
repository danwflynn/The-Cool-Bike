#include "Renderer.h"
#include <glad/glad.h>

void Renderer::Init() {
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

void Renderer::BeginFrame() {
    glClearColor(0.1f, 0.2f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::EndFrame() {
    // later: render queues, post-processing, UI
}
