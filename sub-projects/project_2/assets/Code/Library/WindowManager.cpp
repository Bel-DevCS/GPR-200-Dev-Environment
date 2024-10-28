//
// Created by Kearura on 2024-10-27.
//

#include "WindowManager.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Bella_Engine
{
    GLFWwindow* Window::InitializeWindow(int width, int height, const char* title)
    {
        printf("Initializing...");
        if (!glfwInit())
        {
            printf("GLFW failed to init!");
            return nullptr;
        }

        // Set OpenGL context hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create window
        GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (window == nullptr) {
            printf("GLFW failed to create window");
            glfwTerminate();
            return nullptr;
        }
        glfwMakeContextCurrent(window);

        if (!gladLoadGL(glfwGetProcAddress)) {
            printf("GLAD Failed to load GL headers");
            glfwTerminate();
            return nullptr;
        }

        return window;
    }

    void Window::InitImGui(GLFWwindow* window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330 core");
    }

    void Window::Terminate(GLFWwindow* window)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(window);
        glfwTerminate();
    }

}
