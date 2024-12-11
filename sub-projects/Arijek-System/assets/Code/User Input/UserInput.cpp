//
// Created by Kearura on 2024-10-26.
//

#include "UserInput.h"

void UserInput::processInput(GLFWwindow* window, Bella_GPR200::Camera& camera, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.KeyboardInput(Bella_GPR200::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.KeyboardInput(Bella_GPR200::DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.KeyboardInput(Bella_GPR200::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.KeyboardInput(Bella_GPR200::RIGHT, deltaTime);
}



