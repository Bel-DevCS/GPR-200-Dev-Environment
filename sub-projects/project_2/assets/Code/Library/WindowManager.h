//
// Created by Kearura on 2024-10-27.
//

#pragma once
#include <ew/external/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace Bella_Engine
{
    class Window
    {
    public:
        static GLFWwindow* InitializeWindow(int width, int height, const char* title);
        static void InitImGui(GLFWwindow* window);
        static void Terminate(GLFWwindow* window);
    };
}