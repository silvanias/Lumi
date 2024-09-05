#include "render/init/quad_texture.h"
#include "render/init/world.h"
#include "render/gui/window/window.h"
#include "render/shaders/shader.h"
#include "render/render.h"
#include "render/gui/imgui/lifecycle/imgui_lifecycle.h"

int main()
{
  const unsigned int WIDTH = 500;
  const unsigned int HEIGHT = 500;
  const unsigned int IMAGE_SIZE = WIDTH * HEIGHT;
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

  HittableList world = createWorld();

  const auto CAMERA_POS = glm::vec3(-3.0f, 3.0f, 1.0f);
  const auto CAMERA_FOCAL_POINT = glm::vec3(0.0f, 0.0f, 0.0f);
  const auto CAMERA_UP_VEC = glm::vec3(0.0f, 1.0f, 0.0f);
  const float VERT_FOV = 1.2f;
  const unsigned int SAMPLE_PER_PIXEL = 10;
  const unsigned int MAX_DEPTH = 1000;

  Camera camera(WIDTH, HEIGHT, CAMERA_POS, CAMERA_FOCAL_POINT, CAMERA_UP_VEC, VERT_FOV, SAMPLE_PER_PIXEL, MAX_DEPTH);
  renderLoop(window, shader, quadVAO, texture, world, camera, IMAGE_SIZE);

  ImGuiShutdown();
  glfwShutdown(window);
  return 0;
}
