#include "Renderer.h"
#include <glad/glad.h>

void Renderer::Init() {
    glClearColor(0.1f, 0.2f, 0.8f, 1.0f);
}

void Renderer::Render() {
    glClear(GL_COLOR_BUFFER_BIT);
}
