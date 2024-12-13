//
// Created by anne.konicki on 11/12/2024.
//

#include "Scene.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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

            // Draw call on all particle systems
            for (auto& particleGenerator : mParticleGenerators) {
                particleGenerator.Update(deltaTime, 50, 50);
                particleGenerator.Render(view, projection);
            }

            //mTerrainGenerator.Render(mCamera, view, projection,SCREEN_WIDTH, SCREEN_HEIGHT);

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
            exit(1);
        }

        mWindow = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Final Project", nullptr, nullptr);
        if (!mWindow) {
            printf("GLFW failed to create window");
            exit(1);
        }
        glfwMakeContextCurrent(mWindow);
        if (!gladLoadGL(glfwGetProcAddress)) {
            printf("GLAD Failed to load GL headers");
            exit(1);
        }

        glfwSetScrollCallback(mWindow, Scene::scroll_callback);
        glfwSetCursorPosCallback(mWindow, Scene::mouse_callback);
        glfwSetFramebufferSizeCallback(mWindow, [](GLFWwindow* window, int width, int height)
        {
            glViewport(0, 0, width, height);
        });
        glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        mCamera = ak::Camera(mWindow, glm::vec3(0.0f, 0.0f, 1.0f));

        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);

        // ImGUI Init
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
        ImGui_ImplOpenGL3_Init("#version 330 core");
    }

    void Scene::drawUI() {
        // TODO: Implement based on project specifics. Likely will not abstract functionality

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        for (auto& model : mModels) {
            if (ImGui::Begin((model.getName() + " Settings").c_str())) {
                ImGui::SetWindowSize(ImVec2(500, 125));
                // Temporary copies for user input
                glm::vec3 newPosition = model.getPosition();
                glm::vec3 newRotation = model.getRotation();
                glm::vec3 newScale = model.getScale();
                bool newIsVisible = model.getVisibility();

                if (ImGui::SliderFloat3("Position", &newPosition[0], -1.0f, 1.0f)) {
                    model.setPosition(newPosition); // Update temporary model settings on change
                }

                if (ImGui::SliderFloat3("Rotation", &newRotation[0], 0.0f, 360.0f)) {
                    model.setRotation(newRotation); // Update temporary model settings on change
                }

                if (ImGui::SliderFloat3("Scale", &newScale[0], 0.01f, 3.0f)) {
                    model.setScale(newScale); // Update temporary model settings on change
                }

                if (ImGui::Checkbox("Visible", &newIsVisible)) {
                    model.setVisibility(newIsVisible); // Update temporary model settings on change
                }

                ImGui::End();
            }
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Scene::scroll_callback(GLFWwindow *window, double xOffset, double yOffset) {
    }

    void Scene::mouse_callback(GLFWwindow *window, double xPos, double yPos) {
    }
} // final_project