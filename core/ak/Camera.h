#include <glm/glm.hpp>
#include "GLFW/glfw3.h"

#ifndef CAMERA_H
#define CAMERA_H

#define CAMERA_SENSITIVITY 0.1
#define MAX_ZOOM 1.0f
#define MIN_ZOOM 120.0f

namespace ak {


    // Camera Class by Anne
class Camera {
public:
    Camera();
    Camera(GLFWwindow* window);
    Camera(GLFWwindow* window, glm::vec3 initialPosition);
    void update(float deltaTime);
    void updateLocalAngles();
    glm::mat4 getView() const;
    glm::mat4 getProjection() const;
    void setWindow(GLFWwindow* window) { mWindow = window; }
    GLFWwindow* getWindow() const { return mWindow; }
    void processMouseMovement(double xOffset, double yOffset);
    void processMouseScroll(double yOffset);
    float getZoom() const { return mZoom; }

private:
    glm::vec3 mPosition = glm::vec3(0, 0, 3.0f);
    glm::vec3 mTarget = glm::vec3(0, 0, -1.0f);
    glm::vec3 mWorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 mLocalRight;
    glm::vec3 mLocalUp;
    glm::vec3 mLocalForward = mTarget;

    double lastX, lastY = 0;
    float mYaw = -90.0f;
    float mPitch = 0.0f;
    float mZoom = 60.0f;

    bool mPanning = false;

    void processKeyboardInput(float deltaTime);
    void processMouseInput(float deltaTime);

    GLFWwindow* mWindow;

    float mSpeed = 0.05f;

    bool firstMouse = true;
};

} // ak

#endif //CAMERA_H
