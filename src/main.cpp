#define GLFW_DLL

#include <array>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "render/gui/window/window.h"
#include "render/shaders/shader.h"
#include "render/render.h"
#include "render/gui/imgui/lifecycle/imgui_lifecycle.h"
#include "render/material.h"

void initQuad(unsigned int &VAO, unsigned int &VBO)
{
  std::array<float, 24>
      quadVertices = {
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

void configureTexture(unsigned int &texture, const int &WIDTH, const int &HEIGHT)
{
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

int main()
{
  const unsigned int WIDTH = 1000;
  const unsigned int HEIGHT = 800;
  const unsigned int IMAGE_SIZE = WIDTH * HEIGHT;

  const glm::vec3 CAMERA_POS = glm::vec3(0.0f, 0.0f, 1.0f);
  const glm::vec3 CAMERA_FOCAL_POINT = glm::vec3(0.0f, 0.0f, -1.0f);
  const glm::vec3 CAMERA_UP_VEC = glm::vec3(0.0f, 1.0f, 0.0f);
  const float VERT_FOV = 1.4f;

  const unsigned int SAMPLE_PER_PIXEL = 1;
  const unsigned int MAX_DEPTH = 1000;

  GLFWwindow *window = createWindow(WIDTH, HEIGHT);
  configWindow(window);
  initializeGlAD();
  initImGui(window);

  Shader shader("../src/render/shaders/vertex.vs", "../src/render/shaders/fragment.fs");

  unsigned int quadVAO;
  unsigned int quadVBO;
  initQuad(quadVAO, quadVBO);

  unsigned int texture;
  configureTexture(texture, WIDTH, HEIGHT);

  HittableList world;

  auto material_ground = std::make_shared<Lambertian>(glm::vec3(0.1f, 0.2f, 0.5f));
  auto material_center = std::make_shared<Lambertian>(glm::vec3(0.76f, 0.13f, 0.89f));
  auto material_left = std::make_shared<Metal>(glm::vec3(0.8f, 0.8f, 0.8f));
  auto material_right = std::make_shared<Metal>(glm::vec3(0.8f, 0.6f, 0.2f));

  world.add(std::make_shared<Sphere>(glm::vec3(0, 0, -1.0f), 0.5f, material_center));
  world.add(std::make_shared<Sphere>(glm::vec3(0, -100.5f, -1), 100.0f, material_ground));
  world.add(std::make_shared<Sphere>(glm::vec3(-1.0f, 0, -1.0f), 0.5f, material_left));
  world.add(std::make_shared<Sphere>(glm::vec3(1.0f, 0, -1.0f), 0.5f, material_right));

  Camera camera(WIDTH, HEIGHT, CAMERA_POS, CAMERA_FOCAL_POINT, CAMERA_UP_VEC, VERT_FOV, SAMPLE_PER_PIXEL, MAX_DEPTH);
  renderLoop(window, shader, quadVAO, texture, world, camera, IMAGE_SIZE);
  ImGuiShutdown();
  glfwShutdown(window);
  return 0;
}
