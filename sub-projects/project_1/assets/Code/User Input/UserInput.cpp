//
// Created by Kearura on 2024-10-26.
//

#include "UserInput.h"

void UserInput::processInput(GLFWwindow* window, Bella_GPR200::Camera& camera, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.KeyboardInput(Bella_GPR200::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.KeyboardInput(Bella_GPR200::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.KeyboardInput(Bella_GPR200::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.KeyboardInput(Bella_GPR200::RIGHT, deltaTime);
}



