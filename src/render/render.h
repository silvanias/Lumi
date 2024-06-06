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
                const unsigned int &WIDTH,
                const unsigned int &HEIGHT)
{
    std::vector<glm::vec3> accumulationBuffer(WIDTH * HEIGHT, glm::vec3(0.0f));
    std::vector<int> sampleCount(WIDTH * HEIGHT, 0);

    HittableList world;
    world.add(make_shared<Sphere>(glm::vec3(0, 0, -1), 0.5));
    world.add(make_shared<Sphere>(glm::vec3(0, -100.5, -1), 100));
    Camera cam(WIDTH, HEIGHT);

    while (!glfwWindowShouldClose(window))
    {
        cam.render(world, accumulationBuffer, sampleCount);

        auto clear_color = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
        clearFrame(clear_color);
        std::vector<glm::vec3> currentImage(WIDTH * HEIGHT);
        for (int i = 0; i < WIDTH * HEIGHT; ++i)
        {
            currentImage[i] = accumulationBuffer[i] / static_cast<float>(sampleCount[i]);
        }

        updateTexture(texture, currentImage, WIDTH, HEIGHT);
        shader.use();
        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        renderUI();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}