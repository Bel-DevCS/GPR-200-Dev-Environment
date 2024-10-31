#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Bella_Engine {

    enum Camera_Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN,
        ROTATE_LEFT,
        ROTATE_RIGHT
    };

    enum Projection_Mode {
        PERSPECTIVE,
        ORTHOGRAPHIC
    };

    class Camera {
    public:
        // Camera attributes
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;

        // Euler Angles
        float Yaw;
        float Pitch;

        // Camera options
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;

        // Projection settings
        Projection_Mode projectionMode;
        float nearPlane, farPlane, aspectRatio, orthoSize;

        // Constructor
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
               glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
               float yaw = -90.0f, float pitch = 0.0f,
               float aspectRatio = 4.0f/3.0f, float nearPlane = 0.1f, float farPlane = 100.0f);

        // Core functions
        glm::mat4 GetViewMatrix();
        void SetViewMatrix(const glm::vec3& target);
        glm::mat4 GetProjectionMatrix();
        void KeyboardInput(Camera_Movement direction, float deltaTime);
        void MouseInput(float xoffset, float yoffset, bool constrainPitch = true);
        void ScrollInput(float yoffset);
        void ToggleProjection();
        void Initialize();
        void Cleanup();

        void UpdateCameraVectors(float deltaTime);
    };
}
