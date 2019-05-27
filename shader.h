#include <glad/glad.h>
#include <fmt/core.h>
#include <string>
#include <optional>
#include <iostream>

#pragma once

class Shader {
    GLuint progID;
public:

    Shader(const GLchar* vertexShader, const GLchar* fragmentShader);
    ~Shader();

    void use();
    void setUniform1f(const std::string& name, float value) const;
    void setUniform2f(const std::string& name, float x, float y) const;
    void setUniform3f(const std::string& name, float x, float y, float z) const;
};
