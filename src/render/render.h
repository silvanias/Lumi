#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include "imgui/imgui.h"
#include "gui/window/window.h"
#include "gui/imgui/render/imgui_render.h"
#include "shaders/shader.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "ray.h"
#include "camera.h"

void updateTexture(unsigned int texture, const std::vector<glm::vec3> &image, const int &WIDTH, const int &HEIGHT)
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH, HEIGHT, GL_RGB, GL_FLOAT, image.data());
}

void clearFrame(const ImVec4 &clear_color)
{
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderLoop(GLFWwindow *window,
                const Shader &shader,
                const unsigned int &VAO,
                const unsigned int &texture,
                const HittableList &world,
                Camera &camera,
                const unsigned int &IMAGE_SIZE)
{
    std::vector<glm::vec3> accumulationBuffer(IMAGE_SIZE, glm::vec3(0.0f));
    std::vector<int> sampleCount(IMAGE_SIZE, 0);
    std::vector<glm::vec3> currentImage(IMAGE_SIZE);

    auto clear_color = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        camera.render(world, accumulationBuffer, sampleCount);

        for (int i = 0; i < IMAGE_SIZE; ++i)
        {
            currentImage[i] = accumulationBuffer[i] / static_cast<float>(sampleCount[i]);
            currentImage[i] = Utils::Color::linearToGamma(currentImage[i]);
        }
        clearFrame(clear_color);
        updateTexture(texture, currentImage, camera.image_width, camera.image_height);

        shader.use();
        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // renderUI();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}