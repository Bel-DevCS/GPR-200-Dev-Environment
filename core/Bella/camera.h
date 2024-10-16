#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Bella_GPR200
{
    enum Camera_Movement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN,
        ROTATE_LEFT,
        ROTATE_RIGHT
    };

    class Camera
    {
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

        // Constructor
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
               glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
               float yaw = -90.0f, float pitch = 0.0f);

        // Returns the view matrix calculated using LookAt matrix
        glm::mat4 GetViewMatrix();

        // Renamed input functions for clarity
        void KeyboardInput(Camera_Movement direction, float deltaTime);
        void MouseInput(float xoffset, float yoffset, bool constrainPitch = true);
        void ScrollInput(float yoffset);

    private:
        // Updates the camera vectors based on the current values of Yaw and Pitch
        void updateCameraVectors();
    };

};
