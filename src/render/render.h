#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include "imgui/imgui.h"
#include "gui/window/window.h"
#include "gui/imgui/render/imgui_render.h"
#include "shaders/shader.h"
#include "geometry/hittable/hittable.h"
#include "geometry/hittable/hittable_list.h"
#include "geometry/objects/sphere.h"
#include "geometry/ray.h"
#include "camera/camera.h"
#include <chrono>
#include <iostream>

class Render
{
public:
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

        double totalRenderTime = 0.0;
        int numRuns = 0;

        while (!glfwWindowShouldClose(window))
        {
            auto start = std::chrono::high_resolution_clock::now();

            camera.render(world, accumulationBuffer, sampleCount);

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;

            totalRenderTime += elapsed.count();
            ++numRuns;

            double averageRenderTime = totalRenderTime / numRuns;

            std::cout << "Current render time: " << elapsed.count() << " seconds, "
                      << "Average render time: " << averageRenderTime << " seconds, "
                      << "Number of runs: " << numRuns << std::endl;

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

            renderUI();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
};