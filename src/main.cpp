#define GLFW_DLL

#include <array>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "render/gui/window/window.h"
#include "render/shaders/shader.h"
#include "render/render.h"
#include "render/gui/imgui/lifecycle/imgui_lifecycle.h"

void initQuad(unsigned int &VAO, unsigned int &VBO)
{
  std::array<float, 24>
      quadVertices = {
          // positions     // texCoords
          -1.0f, 1.0f, 0.0f, 1.0f,
          -1.0f, -1.0f, 0.0f, 0.0f,
          1.0f, -1.0f, 1.0f, 0.0f,

          -1.0f, 1.0f, 0.0f, 1.0f,
          1.0f, -1.0f, 1.0f, 0.0f,
          1.0f, 1.0f, 1.0f, 1.0f};

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)nullptr);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

  glBindVertexArray(0);
}

const int WIDTH = 500;
const int HEIGHT = 500;
const int MAX_ITERATIONS = 1000;
const int DISPLAY_INTERVAL = 10;

int main()
{
  GLFWwindow *window = createWindow(WIDTH, HEIGHT);
  configWindow(window);
  initializeGlAD();
  initImGui(window);

  Shader shader("../src/render/shaders/vertex.vs", "../src/render/shaders/fragment.fs");
  unsigned int quadVAO;
  unsigned int quadVBO;
  initQuad(quadVAO, quadVBO);

  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  renderLoop(window, shader, quadVAO, texture, WIDTH, HEIGHT, MAX_ITERATIONS, DISPLAY_INTERVAL);
  ImGuiShutdown();
  glfwShutdown(window);
  return 0;
}
