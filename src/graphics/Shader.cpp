#include "Shader.h"
#include <glad/glad.h>
#include <iostream>

unsigned int Shader::CompileShader(unsigned int type, const char* src) {
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(id, 512, nullptr, infoLog);
        std::cerr << "Shader compile error:\n" << infoLog << std::endl;
    }

    return id;
}

Shader::Shader(const char* vertexSrc, const char* fragmentSrc) {
    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSrc);
    unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Shader link error:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    if (program) {
        glDeleteProgram(program);
    }
}

void Shader::Bind() const {
    glUseProgram(program);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

int Shader::GetUniformLocation(const char* name) const {
    if (m_UniformLocationCache.count(name)) {
        return m_UniformLocationCache[name];
    }

    int location = glGetUniformLocation(program, name);
    if (location == -1) {
        std::cerr << "Warning: uniform '" << name << "' doesn't exist or is unused\n";
    }

    m_UniformLocationCache[name] = location;
    return location;
}

void Shader::SetMat4(const char* name, const glm::mat4& matrix) const {
    glUniformMatrix4fv(
        GetUniformLocation(name),
        1,
        GL_FALSE,
        glm::value_ptr(matrix)
    );
}

void Shader::SetVec3(const char* name, const glm::vec3& value) const {
    int location = glGetUniformLocation(program, name);
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::SetFloat(const char* name, float value) const {
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetInt(const char* name, int value) const {
    glUniform1i(GetUniformLocation(name), value);
}
