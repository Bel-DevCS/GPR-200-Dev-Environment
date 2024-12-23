//
// Created by Kearura on 2024-10-26.
// Modified for Project 5 on 2024-10-31.
//

#pragma once

#include <GLFW/glfw3.h>
#include "Bella/Mechanic/Camera/camera.h"

// Struct to hold references to UserInput and Camera

class UserInput
{
public:
    UserInput();

    void processInput(GLFWwindow* window, Bella_GPR200::Camera& camera, float deltaTime);
    void handleProjectionToggle(GLFWwindow* window);
    void handleMouseLockToggle(GLFWwindow* window);

    // Static callback functions
    static void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

    [[nodiscard]] bool isPerspectiveEnabled() const { return isPerspective; }
    [[nodiscard]] glm::mat4 getProjectionMatrix(const Bella_GPR200::Camera& camera,float aspectRatio) const;

    bool isMouseLocked;

private:
    float lastX;
    float lastY;
    bool firstMouse;
    bool isPerspective;

};

struct InputContext
{
    Bella_GPR200::Camera* camera;
    UserInput* userInput;
};

