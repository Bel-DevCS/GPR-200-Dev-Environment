//
// Created by anne.konicki on 11/12/2024.
//

#ifndef SCENE_H
#define SCENE_H
#include <vector>

#include "../Camera/camera.h"
#include "./../../../ak/Camera.h"
#include "../Model/model.h"
#include "GLFW/glfw3.h"

namespace final_project {

class Scene {
public:
    Scene();

    void play();
    void initialize();

private:
    void drawUI();

    std::vector<Bella_GPR200::Model> mModels;
    ak::Camera mCamera;
    GLFWwindow* mWindow = nullptr;

    static constexpr unsigned int SCREEN_WIDTH = 1080;
    static constexpr unsigned int SCREEN_HEIGHT = 720;
    static constexpr float NEAR_PLANE = 0.1f;
    static constexpr float FAR_PLANE = 1000.0f;

    const std::string mSCENE_NAME = "Scene";

    glm::vec3 mClearScreenColor = glm::vec3(0.3, 0.4, 0.9);
    auto mClearBit = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

    static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
    static void mouse_callback(GLFWwindow* window, double xPos, double yPos);
};

} // final_project

#endif //SCENE_H
