#pragma once

#include <string>

class Shader {
public:
    Shader(const char* vertexSrc, const char* fragmentSrc);
    ~Shader();

    void Bind() const;
    void Unbind() const;

private:
    unsigned int program;

    unsigned int CompileShader(unsigned int type, const char* src);
};
