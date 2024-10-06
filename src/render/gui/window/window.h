#pragma once

#include <GLFW/glfw3.h>

GLFWwindow *createWindow(GLint scr_width = 800, GLint scr_height = 600);
void configWindow(GLFWwindow *window);

// Initializes GLAD for OpenGL function loading.
bool initializeGlAD();

void glfwShutdown(GLFWwindow *window);