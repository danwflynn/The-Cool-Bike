#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    Shader(const char* vertexSrc, const char* fragmentSrc);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // Uniform setters
    void SetMat4(const char* name, const glm::mat4& matrix) const;
    void SetVec3(const char* name, const glm::vec3& value) const;
    void SetFloat(const char* name, float value) const;
    void SetInt(const char* name, int value) const;

private:
    unsigned int program;

    unsigned int CompileShader(unsigned int type, const char* src);

    int GetUniformLocation(const char* name) const;

private:
    mutable std::unordered_map<std::string, int> m_UniformLocationCache;
};
