#pragma once

class Mesh {
public:
    Mesh(const float* vertices, unsigned int size);
    ~Mesh();

    void Draw() const;

private:
    unsigned int vao;
    unsigned int vbo;
};
