#pragma once

#include <glad/glad.h>
#include <cstddef>

class Mesh {
public:
    Mesh(
        const float* vertices,
        size_t vertexSize,
        const unsigned int* indices,
        size_t indexSize,
        unsigned int strideBytes = 6 * sizeof(float) // default: pos + normal
    );

    ~Mesh();

    void Draw() const;

private:
    unsigned int vao = 0;
    unsigned int vbo = 0;
    unsigned int ebo = 0;

    size_t indexCount = 0;
    unsigned int vertexStride = 0;
};
