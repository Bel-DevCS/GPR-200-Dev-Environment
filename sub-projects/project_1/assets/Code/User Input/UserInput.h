//
// Created by Kearura on 2024-10-26.
//

#pragma once
#include <GLFW/glfw3.h>
#include "Bella/Mechanic/Camera/camera.h"

class UserInput
{
    public:
    void processInput(GLFWwindow* window, Bella_GPR200::Camera& camera, float deltaTime);
    void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
};
