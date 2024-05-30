#pragma once

#include "gui/window/window.h"
#include "gui/imgui/render/imgui_render.h"

#include "imgui/imgui.h"

void clearFrame(const ImVec4 &clear_color)
{
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderLoop(GLFWwindow *window, const Shader &shader, const unsigned int &VAO)
{
    auto clear_color = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        clearFrame(clear_color);
        renderUI();
        shader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
