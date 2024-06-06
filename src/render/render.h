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

glm::vec3 rayColor(const Ray &r, const HittableList &world)
{
    HitRecord rec;
    if (world.hit(r, 0, INFINITY, rec))
    {
        return 0.5f * (rec.normal + glm::vec3(1.0f, 1.0f, 1.0f));
    }
    else
    {
        glm::vec3 unit_direction = glm::normalize(r.direction());
        auto factor = 0.5f * (unit_direction.y + 1.0f);
        auto lerp = (1.0f - factor) * glm::vec3(1.0f, 1.0f, 1.0f) + factor * glm::vec3(0.5, 0.7, 1.0);
        return lerp;
    }
}

void renderLoop(GLFWwindow *window,
                const Shader &shader,
                const unsigned int &VAO,
                unsigned int &texture,
                const unsigned int &WIDTH,
                const unsigned int &HEIGHT)
{
    std::vector<glm::vec3> accumulationBuffer(WIDTH * HEIGHT, glm::vec3(0.0f));
    std::vector<int> sampleCount(WIDTH * HEIGHT, 0);

    // World

    HittableList world;
    world.add(make_shared<Sphere>(glm::vec3(0, 0, -1), 0.5));
    world.add(make_shared<Sphere>(glm::vec3(0, -100.5, -1), 100));

    // Camera
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * ((float)WIDTH / (float)HEIGHT);
    auto camera_center = glm::vec3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = glm::vec3(viewport_width, 0, 0);
    auto viewport_v = glm::vec3(0, viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    auto pixel_delta_u = viewport_u / (float)WIDTH;
    auto pixel_delta_v = viewport_v / (float)HEIGHT;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left = camera_center - glm::vec3(0, 0, focal_length) - viewport_u / 2.0f - viewport_v / 2.0f;
    auto pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);

    while (!glfwWindowShouldClose(window))
    {
        for (int y = 0; y < HEIGHT; ++y)
        {
            for (int x = 0; x < WIDTH; ++x)
            {
                auto pixel_center = pixel00_loc + (float(x) * pixel_delta_u) + (float(y) * pixel_delta_v);
                auto ray_direction = pixel_center - camera_center;
                Ray r(camera_center, ray_direction);

                glm::vec3 pixel_color = rayColor(r, world);
                int index = y * WIDTH + x;
                accumulationBuffer[index] += pixel_color;
                sampleCount[index] += 1;
            }
        }

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