#pragma once
#include <string>
#include <glm/gtc/type_ptr.hpp>


class Shader {
public:
    Shader(const char* vertexSrc, const char* fragmentSrc);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetMat4(const char* name, const glm::mat4& matrix) const;

private:
    unsigned int program;

    unsigned int CompileShader(unsigned int type, const char* src);
};
