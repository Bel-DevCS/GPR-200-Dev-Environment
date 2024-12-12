#include "camera.h"

namespace Bella_GPR200
{
    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
            : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(2.5f), MouseSensitivity(0.1f), Zoom(45.0f)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    Camera::Camera(glm::vec3 position, glm::vec3 up, float orthoSize, bool isIsometric)
       : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(2.5f), MouseSensitivity(0.1f), Zoom(10.0f), OrthoSize(orthoSize), IsIsometric(isIsometric)
    {
        if (isIsometric)
        {
            Position = glm::vec3(10.0f, 10.0f, 10.0f); // Default isometric position
            Yaw = -45.0f;
            Pitch = -35.26f;
        }
        else
        {
            Position = position;
            Yaw = -90.0f;
            Pitch = 0.0f;
        }

        WorldUp = up;
        updateCameraVectors();
    }


    glm::mat4 Camera::GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    glm::mat4 Camera::GetProjectionMatrix(float aspectRatio)
    {
        if (IsIsometric)
        {
            float halfSize = OrthoSize / 2.0f;
            return glm::ortho(-halfSize * aspectRatio, halfSize * aspectRatio, -halfSize, halfSize, 0.1f, 100.0f);
        }
        else
        {
            return glm::perspective(glm::radians(Zoom), aspectRatio, 0.1f, 100.0f);
        }
    }

    // Renamed to KeyboardInput
    void Camera::KeyboardInput(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;

        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
        if (direction == UP)
            Position += WorldUp * velocity;
        if (direction == DOWN)
            Position -= WorldUp * velocity;

        if (direction == ROTATE_LEFT)
            Yaw -= 45.0f * deltaTime;

        if (direction == ROTATE_RIGHT)
            Yaw += 45.0f * deltaTime;

        // Update camera vectors after modifying yaw/pitch
        updateCameraVectors();
    }

    // Renamed to MouseInput
    void Camera::MouseInput(float xoffset, float yoffset, bool constrainPitch)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if (constrainPitch) {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // Update camera vectors after mouse movement
        updateCameraVectors();
    }

    // Renamed to ScrollInput
    void Camera::ScrollInput(float yoffset)
    {
        Zoom -= yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

    void Camera::updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);

        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }

}
