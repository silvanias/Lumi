#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include "imgui/imgui.h"
#include "gui/window/window.h"
#include "gui/imgui/render/imgui_render.h"
#include "shaders/shader.h"
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

float hit_sphere(const glm::vec3 &center, const float &radius, const ray &r)
{
    glm::vec3 oc = center - r.origin();
    float a = glm::length2(r.direction());
    auto h = dot(r.direction(), oc);
    auto c = glm::length2(oc) - radius * radius;
    auto discriminant = h * h - a * c;

    if (discriminant < 0)
    {
        return -1.0f;
    }
    else
    {
        return (h - sqrt(discriminant)) / a;
    }
};

glm::vec3 rayColor(const ray &r)
{
    auto sphere_pos = glm::vec3(0, 0, -1);
    auto sphere_radius = 0.5f;
    auto hit_point = hit_sphere(sphere_pos, sphere_radius, r);
    if (hit_point > 0.0f)
    {
        glm::vec3 normal = glm::normalize(r.at(hit_point) - sphere_pos);
        return 0.5f * glm::vec3(normal.x + 1, normal.y + 1, normal.z + 1);
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
                ray r(camera_center, ray_direction);

                glm::vec3 pixel_color = rayColor(r);
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
