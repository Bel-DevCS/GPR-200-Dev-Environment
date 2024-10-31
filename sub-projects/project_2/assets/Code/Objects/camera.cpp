#include "camera.h"

#include <iostream>

namespace Bella_Engine {

    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch,
                   float aspectRatio, float nearPlane, float farPlane)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(2.5f), MouseSensitivity(0.1f), Zoom(45.0f),
          projectionMode(PERSPECTIVE), orthoSize(10.0f), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        UpdateCameraVectors(0.0f);  // Initial update
    }

    glm::mat4 Camera::GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void Camera::SetViewMatrix(const glm::vec3& target)
    {
        // Update the Front vector to look at the target
        Front = glm::normalize(target - Position);

        // Recalculate Right and Up vectors
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }


    glm::mat4 Camera::GetProjectionMatrix() {
        if (projectionMode == PERSPECTIVE) {
            return glm::perspective(glm::radians(Zoom), aspectRatio, nearPlane, farPlane);
        } else { // ORTHOGRAPHIC
            float orthoWidth = orthoSize * aspectRatio;
            return glm::ortho(-orthoWidth, orthoWidth, -orthoSize, orthoSize, nearPlane, farPlane);
        }
    }

    void Camera::KeyboardInput(Camera_Movement direction, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD) Position += Front * velocity;
        if (direction == BACKWARD) Position -= Front * velocity;
        if (direction == LEFT) Position -= Right * velocity;
        if (direction == RIGHT) Position += Right * velocity;
        if (direction == UP) Position += WorldUp * velocity;
        if (direction == DOWN) Position -= WorldUp * velocity;
        if (direction == ROTATE_LEFT) Yaw -= 45.0f * deltaTime;
        if (direction == ROTATE_RIGHT) Yaw += 45.0f * deltaTime;

        UpdateCameraVectors(deltaTime);
    }

    void Camera::MouseInput(float xoffset, float yoffset, bool constrainPitch) {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if (constrainPitch) {
            if (Pitch > 89.0f) Pitch = 89.0f;
            if (Pitch < -89.0f) Pitch = -89.0f;
        }

        UpdateCameraVectors(0.0f);  // No deltaTime needed here
    }

    void Camera::ScrollInput(float yoffset) {
        Zoom -= yoffset;
        if (Zoom < 1.0f) Zoom = 1.0f;
        if (Zoom > 45.0f) Zoom = 45.0f;
    }

    void Camera::ToggleProjection() {
        projectionMode = (projectionMode == PERSPECTIVE) ? ORTHOGRAPHIC : PERSPECTIVE;
    }

    void Camera::Initialize() {
        std::cout << "Camera initialized at position: "
                  << Position.x << ", " << Position.y << ", " << Position.z << std::endl;
    }

    void Camera::Cleanup() {
        // Reset camera attributes to default values
        Position = glm::vec3(0.0f, 0.0f, 3.0f);
        Front = glm::vec3(0.0f, 0.0f, -1.0f);
        Up = glm::vec3(0.0f, 1.0f, 0.0f);
        Right = glm::vec3(1.0f, 0.0f, 0.0f);
        WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        Yaw = -90.0f;
        Pitch = 0.0f;
        MovementSpeed = 2.5f;
        MouseSensitivity = 0.1f;
        Zoom = 45.0f;
        projectionMode = PERSPECTIVE;
        nearPlane = 0.1f;
        farPlane = 100.0f;
        orthoSize = 10.0f;

        std::cout << "Camera cleanup complete, reset to default values." << std::endl;
    }

    void Camera::UpdateCameraVectors(float deltaTime) {
        // Smooth transitions based on deltaTime
        float adjustedYaw = Yaw * deltaTime;
        float adjustedPitch = Pitch * deltaTime;

        glm::vec3 front;
        front.x = cos(glm::radians(adjustedYaw)) * cos(glm::radians(adjustedPitch));
        front.y = sin(glm::radians(adjustedPitch));
        front.z = sin(glm::radians(adjustedYaw)) * cos(glm::radians(adjustedPitch));

        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));  // Recalculate Right vector
        Up = glm::normalize(glm::cross(Right, Front));  // Recalculate Up vector
    }
}
