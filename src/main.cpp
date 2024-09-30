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

  HittableList world = cornellBox();

  // For full use must be a square number (stratification in square grid)
  const unsigned int SAMPLE_PER_PIXEL = 1;
  const unsigned int MAX_DEPTH = 1000;

  Camera camera(WIDTH, HEIGHT, CAM_POS_CORNELL_BOX, SAMPLE_PER_PIXEL, MAX_DEPTH);
  auto renderer = Render();
  renderer.renderLoop(window, shader, quadVAO, texture, world, camera, IMAGE_SIZE);

  ImGuiShutdown();
  glfwShutdown(window);
  return 0;
}
