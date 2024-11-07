#pragma once
#include <ew/external/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "../scripts/AssetManager/AssetManager.h"

class Program
{
public:
    void Rise();
    void RenderLoop();
    void Terminate();

    AssetManager assetManager;
private:
    void InitWindow();
    void InitImGui();
    void Update();      // New: Handles updates like input and logic
    void Draw();        // Draws OpenGL content
    void RenderUI();    // New: Handles ImGui rendering

    GLFWwindow* window;
    bool isRunning = true;

    int width = 1080;
    int height = 720;
    const char* title = "Project 3 : Modular Shaders";
};

