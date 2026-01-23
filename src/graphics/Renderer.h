#pragma once

class Renderer {
public:
    void Init();
    void BeginFrame();
    void EndFrame();

private:
    unsigned int vao = 0;
    unsigned int vbo = 0;
    unsigned int shaderProgram = 0;
};
