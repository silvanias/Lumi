#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window.h"

GLFWwindow *createWindow(GLint scr_width, GLint scr_height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // safe on mac
    GLFWwindow *window = glfwCreateWindow(scr_width, scr_height, "lumi", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    return window;
}

void configWindow(GLFWwindow *window)
{
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
}

bool initializeGlAD()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;
}

void glfwShutdown(GLFWwindow *window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
}