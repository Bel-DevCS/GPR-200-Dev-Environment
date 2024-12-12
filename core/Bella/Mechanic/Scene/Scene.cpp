//
// Created by anne.konicki on 11/12/2024.
//

#include "Scene.h"

#include <glm/ext/matrix_clip_space.hpp>

namespace final_project {
    Scene::Scene() {
        // Does this need any implementation? I dont actually think anything needs to happen here
        initialize();
    }

    void Scene::play() {
        float deltaTime, lastFrame = 0;

        // Normal scene loop
        while (!glfwWindowShouldClose(mWindow)) {
            glfwPollEvents();

            glClearColor(mClearScreenColor.r, mClearScreenColor.g, mClearScreenColor.b, 1.0f);
            glClear(mClearBit);

            float currentTime = glfwGetTime();
            deltaTime = currentTime - lastFrame;
            lastFrame = currentTime;

            mCamera.update(deltaTime);

            auto view = mCamera.getView();
            auto projection = glm::perspective(glm::radians(mCamera.getZoom()), float(SCREEN_WIDTH) / float (SCREEN_HEIGHT), NEAR_PLANE, FAR_PLANE);

            // Draw call on all models
            for (auto& model : mModels) {
                model.getShader().use();

                model.getShader().setMat4("view", view);
                model.getShader().setMat4("projection", projection);

                // Set lighting onto models for each light in the scene
                for (auto& light : mLights) {
                    light.SetLightUniforms(model.getShader());
                }

                model.Draw();
            }

            drawUI();

            glfwSwapBuffers(mWindow);
        }
    }

    void Scene::initialize() {
        printf("Initializing scene...");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        printf("Initializing OpenGL context...");
        if (!glfwInit()) {
            printf("GLFW failed to init!");
            return;
        }

        mWindow = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Final Project", nullptr, nullptr);
        if (!mWindow) {
            printf("GLFW failed to create window");
            return;
        }
        glfwMakeContextCurrent(mWindow);
        if (!gladLoadGL(glfwGetProcAddress)) {
            printf("GLAD Failed to load GL headers");
            return;
        }

        glfwSetScrollCallback(mWindow, Scene::scroll_callback);
        glfwSetCursorPosCallback(mWindow, Scene::mouse_callback);
        glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        mCamera = ak::Camera(mWindow, glm::vec3(0.0f, 0.0f, 1.0f));

        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
    }

    void Scene::drawUI() {
        // TODO: Implement based on project specifics. Likely will not abstract functionality
    }

    void Scene::scroll_callback(GLFWwindow *window, double xOffset, double yOffset) {
    }

    void Scene::mouse_callback(GLFWwindow *window, double xPos, double yPos) {
    }
} // final_project