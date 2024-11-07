#include "UserInput.h"

#include <imgui.h>

UserInput::UserInput()
    : lastX(540.0f), lastY(360.0f), firstMouse(true), isPerspective(true), isMouseLocked(true)
{
}

void UserInput::processInput(GLFWwindow* window, Bella_GPR200::Camera& camera, float deltaTime)
{
    ImGuiIO& io = ImGui::GetIO();

    // Process camera movement if ImGui isn't capturing keyboard input
    if (!io.WantCaptureKeyboard) {
        camera.MovementSpeed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
                                glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) ? 10.0f : 1.0f;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.KeyboardInput(Bella_GPR200::FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.KeyboardInput(Bella_GPR200::BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.KeyboardInput(Bella_GPR200::LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.KeyboardInput(Bella_GPR200::RIGHT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) camera.KeyboardInput(Bella_GPR200::DOWN, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) camera.KeyboardInput(Bella_GPR200::UP, deltaTime);
    }

    // Handle projection toggle only if ImGui isn't capturing keyboard input
    if (!io.WantCaptureKeyboard) {
        handleProjectionToggle(window);
    }

    // Handle mouse lock toggle if ImGui isn't capturing mouse input
    if (!io.WantCaptureMouse) {
        handleMouseLockToggle(window);
    }
}

void UserInput::handleProjectionToggle(GLFWwindow* window)
{
    static bool spacePressed = false;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !spacePressed) {
        isPerspective = !isPerspective;  // Toggle projection mode
        spacePressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
        spacePressed = false;
    }
}

void UserInput::handleMouseLockToggle(GLFWwindow* window)
{
    ImGuiIO& io = ImGui::GetIO();

    // Only toggle if ImGui does NOT want the mouse
    if (!io.WantCaptureMouse && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        isMouseLocked = !isMouseLocked;
        glfwSetInputMode(window, GLFW_CURSOR, isMouseLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }
}

// Static scroll callback
void UserInput::scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
    InputContext* context = static_cast<InputContext*>(glfwGetWindowUserPointer(window));
    if (context && context->camera) {
        context->camera->ScrollInput(static_cast<float>(y_offset));
    }
}

// Static mouse callback
void UserInput::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    InputContext* context = static_cast<InputContext*>(glfwGetWindowUserPointer(window));
    if (context && context->userInput && context->camera) {
        UserInput* userInput = context->userInput;

        if (userInput->firstMouse) {
            userInput->lastX = xpos;
            userInput->lastY = ypos;
            userInput->firstMouse = false;
        }

        float xoffset = xpos - userInput->lastX;
        float yoffset = userInput->lastY - ypos;
        userInput->lastX = xpos;
        userInput->lastY = ypos;

        if (userInput->isMouseLocked) {
            context->camera->MouseInput(xoffset, yoffset);
        }
    }
}

// Get the appropriate projection matrix based on perspective mode
glm::mat4 UserInput::getProjectionMatrix(const Bella_GPR200::Camera& camera, float aspectRatio) const
{
    if (isPerspective) {
        return glm::perspective(glm::radians(camera.Zoom), aspectRatio, 0.1f, 1000.0f);
    }
    else
    {
        return glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 1000.0f);
    }
}
