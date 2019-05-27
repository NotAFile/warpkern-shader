#include "shader.h"

std::string getShaderInfoLog(GLuint shaderId) {
    int  success;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        return std::string(infoLog);
    }
    return "";
}

std::string getProgramInfoLog(GLuint programID) {
    int  success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);

    if(!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
        return std::string(infoLog);
    }
    return "";
}

Shader::Shader(const GLchar* vertexShaderSource, const GLchar* fragmentShaderSource) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    fmt::print("vert: {}\n", getShaderInfoLog(vertexShader));

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    fmt::print("vert: {}\n", getShaderInfoLog(fragmentShader));

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    fmt::print("link: {}\n", getShaderInfoLog(fragmentShader));

    progID = shaderProgram;

    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    fmt::print("create shader {}\n", progID);
}

Shader::~Shader() {
    fmt::print("del shader {}\n", progID);
    glDeleteProgram(progID);
}

void Shader::use() {
    glUseProgram(progID);
}

void Shader::setUniform1f(const std::string& name, float value) const {
    int uniformLocation = glGetUniformLocation(progID, name.c_str());
    glUniform1f(uniformLocation, value);
}

void Shader::setUniform2f(const std::string& name, float x, float y) const {
    int uniformLocation = glGetUniformLocation(progID, name.c_str());
    glUniform2f(uniformLocation, x, y);
}

void Shader::setUniform3f(const std::string& name, float x, float y, float z) const {
    int uniformLocation = glGetUniformLocation(progID, name.c_str());
    glUniform3f(uniformLocation, x, y, z);
}
