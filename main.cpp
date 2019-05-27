#include <glad/glad.h>
#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>
#include <fmt/core.h>
#include <iostream>
#include <fstream>
#include "shader.h"
#include "spi.h"

static const int RES_X = 192;
static const int RES_Y = 12;
// 1.6cm / 50cm
static const float RES_RATIO = 1.6 / 50.0;

bool shouldReloadShader = false;
bool reloadPressed = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    fmt::print("resize! {}x{}\n", width, height);
    // glViewport(0, 0, width, height);
}

void error_callback(int code, const char* description)
{
    fmt::print("GLFW Error: {} (code {})\n", description, code);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        if(!reloadPressed) {
            shouldReloadShader = true;
            reloadPressed = true;
        }
    } else {
        reloadPressed = false;
    }
}

Shader loadAnimShaderFromFile(const std::string& path) {
    std::ifstream fragmentShaderFile("../lib.frag");
    std::string fragmentShaderSource(
            (std::istreambuf_iterator<char>(fragmentShaderFile)),
            std::istreambuf_iterator<char>());

    std::ifstream vertexShaderFile("../basic.vert");
    std::string vertexShaderSource(
            (std::istreambuf_iterator<char>(vertexShaderFile)),
            std::istreambuf_iterator<char>());

    return Shader(vertexShaderSource.c_str(), fragmentShaderSource.c_str());
}

int main(int argc, char** argv) {
    if (!glfwInit())
    {
        fmt::print("init failed!\n");
    }

    fmt::print("started\n");

    glfwSetErrorCallback(error_callback);
    fmt::print("GLFW {}\n", glfwGetVersionString());


    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);


    fmt::print("CreateWindow\n");
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        fmt::print("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    fmt::print("MakeCurrent\n");
    glfwMakeContextCurrent(window);

    if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress))
    {
        fmt::print("Failed to initialize GLAD\n");
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR) {
        fmt::print("OpenGL error: {}", err);
    }

    glViewport(0, 0, RES_X, RES_Y);

    float vertices[] = {
        -1.f, 1.f,
         1.f, 1.f,
         1.f, -1.f,
         -1.f,  -1.f,
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    init_leds();

    fmt::print("Load From File\n");
    // we need this scope for now because the shader must get deleted before
    // glfwTerminate
    {
        Shader myshader = loadAnimShaderFromFile("");
        myshader.use();
        fmt::print("Start Loop\n");

        GLubyte pixbuf[RES_X * RES_Y * 4];

        glPixelStorei(GL_PACK_ALIGNMENT, 0);

        while(!glfwWindowShouldClose(window))
        {
            processInput(window);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            float timeValue = glfwGetTime();
            myshader.setUniform3f("iResolution", RES_X, RES_Y, RES_RATIO);
            myshader.setUniform1f("iTime", timeValue);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

            glReadPixels(0, 0, RES_X, RES_Y, GL_RGBA, GL_UNSIGNED_BYTE, pixbuf);

            glfwSwapBuffers(window);
            glfwPollEvents();

            fmt::print("data: {}\n", pixbuf[1]);

            send_color(RES_Y, RES_X, pixbuf);

            if(shouldReloadShader) {
                myshader = loadAnimShaderFromFile("");
                myshader.use();
                shouldReloadShader = false;
            }
        }
    }

    glfwTerminate();
    return 0;
}
