#include "assets/Code/Library/Scene.h"
#include "assets/Code/Library/WindowManager.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

using namespace Bella_Engine;

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

void RenderAssetManager(std::shared_ptr<Scene> scene);
void ProcessInput(GLFWwindow* window, std::shared_ptr<Camera> camera, float deltaTime);

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "GLFW failed to initialize!" << std::endl;
        return -1;
    }

    // Set OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the window
    GLFWwindow* window = Window::InitializeWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simple Scene");
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    Window::InitImGui(window);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Create a scene
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    // Create a camera and set it as active in the scene
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(
        glm::vec3(0.0f, 0.0f, 10.0f),  // Move camera further back
        glm::vec3(0.0f, 1.0f, 0.0f),
        -90.0f, 0.0f
    );
    scene->SetActiveCamera(camera);

    auto modelShader = std::make_shared<Shader>("assets/Shaders/Generic/genericModel.vert", "assets/Shaders/Generic/genericModel.frag");

    auto model = std::make_shared<Model>("assets/Models/Plant.fbx", modelShader);
    model->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));  // Centered at origin
    model->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));     // Normal scale
    scene->AddObject(model);

    scene->SetActiveCamera(camera);

    auto light = std::make_shared<Light>(Light::CreateDirectional(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f)));
    scene->AddLight(light);

    // Initialize the scene
    scene->Initialize();

    glm::vec3 modelPosition = model->GetPosition();
    std::cout << "Model Position: " << modelPosition.x << ", " << modelPosition.y << ", " << modelPosition.z << std::endl;
    std::cout << "Camera Position: " << camera->Position.x << ", " << camera->Position.y << ", " << camera->Position.z << std::endl;

    // Set initial last frame time
    float lastFrameTime = static_cast<float>(glfwGetTime());

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate deltaTime
        float currentFrameTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        // Process input
        ProcessInput(window, camera, deltaTime);

        // Update the scene
        scene->Update(deltaTime);

        // Render the scene
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene->Render();

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Render ImGui-based asset manager UI
        RenderAssetManager(scene);

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    scene->Cleanup();
    glfwTerminate();

    return 0;
}

void RenderAssetManager(std::shared_ptr<Scene> scene) {
    // ImGui window for asset manager
    ImGui::Begin("Asset Manager");

    // List cameras in the scene
    if (ImGui::CollapsingHeader("Cameras")) {
        auto camera = scene->GetActiveCamera();
        if (camera) {
            ImGui::Text("Active Camera");

            // Dropdown for camera position
            glm::vec3 pos = camera->Position;
            if (ImGui::TreeNode("Position")) {
                ImGui::DragFloat("X", &pos.x, 0.1f);
                ImGui::DragFloat("Y", &pos.y, 0.1f);
                ImGui::DragFloat("Z", &pos.z, 0.1f);
                camera->Position = pos;
                ImGui::TreePop();
            }

            // Additional camera settings (e.g., rotation, zoom)
            float yaw = camera->Yaw;
            float pitch = camera->Pitch;
            if (ImGui::TreeNode("Rotation")) {
                ImGui::DragFloat("Yaw", &yaw, 0.25f);
                ImGui::DragFloat("Pitch", &pitch, 0.25f);
                camera->Yaw = yaw;
                camera->Pitch = pitch;
                ImGui::TreePop();
            }
        }
    }

    if (ImGui::CollapsingHeader("Models")) {
        int modelIndex = 0;
        for (auto& model : scene->GetModels()) {
            ImGui::PushID(modelIndex++);
            if (ImGui::TreeNode("Model")) {
                glm::vec3 position = model->GetPosition();
                if (ImGui::DragFloat3("Position", &position.x, 0.1f)) {
                    model->SetPosition(position);
                }
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
    }

    // Light management
    if (ImGui::CollapsingHeader("Lights")) {
        int lightIndex = 0;
        for (const auto& light : scene->GetLights()) {
            ImGui::PushID(lightIndex++);
            if (ImGui::TreeNode("Light")) {
                glm::vec3 color = light->GetColor();
                ImGui::ColorEdit3("Color", &color[0]);
                light->SetColour(color);

                float intensity = light->GetIntensity();
                ImGui::DragFloat("Intensity", &intensity, 0.1f, 0.0f, 10.0f);
                light->SetIntensity(intensity);

                ImGui::TreePop();
            }
            ImGui::PopID();
        }
    }

    ImGui::End();  // End asset manager window
}

void ProcessInput(GLFWwindow* window, std::shared_ptr<Camera> camera, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->KeyboardInput(Camera_Movement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->KeyboardInput(Camera_Movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->KeyboardInput(Camera_Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->KeyboardInput(Camera_Movement::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera->KeyboardInput(Camera_Movement::DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera->KeyboardInput(Camera_Movement::UP, deltaTime);
}
