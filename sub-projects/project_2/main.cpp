#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

#include "assets/Code/Library/Scene.h"
#include "assets/Code/Library/WindowManager.h"
#include "Bella/Mechanic/shader.h"

using namespace Bella_Engine;

// Constants for screen dimensions
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

int main() {
    // 1. Initialize the window using the custom Window class
    GLFWwindow* window = Window::InitializeWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Demo");
    if (!window) {
        std::cerr << "Failed to create the GLFW window." << std::endl;
        return -1;
    }

    // 2. Initialize ImGui for the created window
    Window::InitImGui(window);

    // 3. Set up the shader
    Bella_GPR200::Shader ourShader("assets/vertexShader.vert", "assets/fragmentShader.frag");

    // 4. Define the vertices for a triangle
    float vertices[] = {
        // X      Y      Z       R     G     B     A
        -0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f, 1.0f,
         0.0f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f, 1.0f
    };

    // 5. Generate and bind the VAO and VBO
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 6. Set the vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 7. Define a variable to store the background color
    float clearColor[4] = { 0.3f, 0.4f, 0.9f, 1.0f };

    // 8. Main render loop
    while (!glfwWindowShouldClose(window)) {
        // 8(a): Apply the current background color
        glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        // 8(b): Update shader uniforms
        float timeValue = glfwGetTime();
        ourShader.setFloat("uTime", timeValue);

        // 8(c): Use the shader program and draw the triangle
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // 8(d): Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 8(e): ImGui window for scene control
        ImGui::Begin("Scenes");

        // Color editor for changing the background color
        ImGui::ColorEdit4("Clear Color", clearColor);

        ImGui::End();

        // 8(f): Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // 8(g): Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 9. Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // 10. Cleanup OpenGL and GLFW
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
