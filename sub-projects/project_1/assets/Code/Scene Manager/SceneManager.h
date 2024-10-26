//
// Created by Kearura on 2024-10-26.
//

#pragma once

#include <ew/external/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class SceneManager
{
    public:
        GLFWwindow* InitializeWindow(int width, int height, const char* title);
        void InitImGui(GLFWwindow* window);
        void Terminate(GLFWwindow* window);
};
