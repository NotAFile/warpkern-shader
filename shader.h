#include <glad/glad.h>
#include <fmt/core.h>
#include <string>
#include <iostream>

#pragma once

class Shader {
    GLuint progID = -1;
public:

    Shader(const GLchar* vertexShader, const GLchar* fragmentShader);
    Shader(){std::cout << "shader empty" << std::endl;};
    ~Shader();

    void use();
    void setUniform1f(const std::string& name, float value) const;
    void setUniform2f(const std::string& name, float x, float y) const;
    void setUniform3f(const std::string& name, float x, float y, float z) const;
};
