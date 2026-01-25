#pragma once
#include <glad/glad.h>
#include <cstddef>

class Mesh {
public:
    // Constructor for indexed mesh (vertices + indices)
    Mesh(const float* vertices, size_t vertexSize, const unsigned int* indices, size_t indexSize);
    ~Mesh();

    void Draw();

private:
    unsigned int vao = 0;
    unsigned int vbo = 0;
    unsigned int ebo = 0;   // Element buffer object (for indices)
    size_t indexCount = 0;
};
