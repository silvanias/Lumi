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

// Render class encapsulates functions for rendering scenes with texture updates and clear routines.
class Render
{
public:
    // Updates a texture with image data.
    void updateTexture(unsigned int texture, const std::vector<glm::vec3> &image, const int &WIDTH, const int &HEIGHT)
    {
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH, HEIGHT, GL_RGB, GL_FLOAT, image.data());
    }

    // Clears the frame with specified color (Necessary for openGL).
    void clearFrame(const ImVec4 &clearColor)
    {
        glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    // Main rendering loop which handles camera rendering, image updates, and UI rendering.
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

        auto clearColor = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);

        double totalRenderTime = 0.0;
        int numRuns = 0;

        // TODO: REALLY NEEDS A REFACTOR, SHOULD BE ENCAPSULATED IN WORLD
        auto empty_material = std::shared_ptr<Material>();
        Quad lights(glm::vec3(343, 554, 332), glm::vec3(-130, 0, 0), glm::vec3(0, 0, -105), empty_material);

        while (!glfwWindowShouldClose(window))
        {
            auto start = std::chrono::high_resolution_clock::now();

            camera.render(world, lights, accumulationBuffer, sampleCount);

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
            clearFrame(clearColor);
            updateTexture(texture, currentImage, camera.imageWidth, camera.imageHeight);

            shader.use();
            glBindVertexArray(VAO);
            glBindTexture(GL_TEXTURE_2D, texture);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            // renderUI();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
};