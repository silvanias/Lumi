#pragma once

#include <vector>

#include "gui/window/window.h"
#include "gui/imgui/render/imgui_render.h"
#include "imgui/imgui.h"

glm::vec3 tracePath(int x, int y, const int &WIDTH, const int &HEIGHT)
{
    auto r = float(x) / float(WIDTH);
    auto g = float(y) / float(HEIGHT);
    auto b = 0.0f;
    return glm::vec3(r, g, b);
}

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
                unsigned int &texture,
                const int &WIDTH,
                const int &HEIGHT,
                const int &MAX_ITERATIONS,
                const int &DISPLAY_INTERVAL)
{
    auto clear_color = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
    std::vector<glm::vec3> accumulationBuffer(WIDTH * HEIGHT, glm::vec3(0.0f));
    std::vector<int> sampleCount(WIDTH * HEIGHT, 0);

    int iteration = 0;
    while (!glfwWindowShouldClose(window))
    {
        for (int y = 0; y < HEIGHT; ++y)
        {
            for (int x = 0; x < WIDTH; ++x)
            {
                glm::vec3 ray = tracePath(x, y, WIDTH, HEIGHT);
                int index = y * WIDTH + x;
                accumulationBuffer[index] += ray;
                sampleCount[index] += 1;
            }
        }

        if (iteration % DISPLAY_INTERVAL == 0)
        {
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
        iteration++;
    }
}
