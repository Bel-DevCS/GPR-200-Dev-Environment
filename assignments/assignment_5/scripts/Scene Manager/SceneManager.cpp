//
// Created by Kearura on 2024-10-26.
//

#include "SceneManager.h"

GLFWwindow* SceneManager::InitializeWindow(int width, int height, const char* title)
{
    printf("Initializing...");
    if (!glfwInit())
    {
        printf("GLFW failed to init!");
        return nullptr;
    }

    // Set OpenGL context hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr) {
        printf("GLFW failed to create window");
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        printf("GLAD Failed to load GL headers");
        glfwTerminate();
        return nullptr;
    }

    return window;
}

void SceneManager::InitImGui(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

//Gui Windows
void SceneManager::LightWindow(Bella_GPR200::Lighting::Light& light)
{
    // Start a new ImGui window
    ImGui::Begin("Light Settings");

    // Retrieve current K values from the light
    float ambientK = light.GetAmbientK();
    float diffuseK = light.GetDiffuseK();
    float specularK = light.GetSpecularK();
    float shininess = light.GetShininess();

    // ImGui sliders for K values
    ImGui::Text("K Values");
    ImGui::Separator();

    if (ImGui::SliderFloat("Ambient K", &ambientK, 0.0f, 1.0f)) {
        light.SetAmbientK(ambientK);
    }

    if (ImGui::SliderFloat("Diffuse K", &diffuseK, 0.0f, 1.0f)) {
        light.SetDiffuseK(diffuseK);
    }

    if (ImGui::SliderFloat("Specular K", &specularK, 0.0f, 1.0f)) {
        light.SetSpecularK(specularK);
    }

    if (ImGui::SliderFloat("Shininess", &shininess, 1.0f, 128.0f)) {
        light.SetShininess(shininess);
    }

    ImGui::Spacing();
    ImGui::Spacing();

    // Light color
    ImGui::Text("Light Color");
    ImGui::Separator();
    float color[3] = {light.GetColor().r, light.GetColor().g, light.GetColor().b};
    if (ImGui::ColorEdit3("Light Color", color)) {
        light.SetColour(glm::vec3(color[0], color[1], color[2]));
    }

    ImGui::Spacing();
    ImGui::Spacing();

    // Light position controls
    switch (light.GetType())
    {
    case Bella_GPR200::Lighting::LightType::DIRECTIONAL:
        {
            ImGui::Text("Directional Light Settings");
            ImGui::Separator();

            // Direction Controls
            glm::vec3 direction = light.GetDirection();
            if (ImGui::DragFloat3("Direction", &direction[0], 0.1f, -1.0f, 1.0f))
            {
                light.SetDirection(direction);
            }
            break;
        }

    case Bella_GPR200::Lighting::LightType::POINT:
        {
            ImGui::Text("Point Light Settings");
            ImGui::Separator();

            // Position Controls
            glm::vec3 position = light.GetPosition();
            if (ImGui::DragFloat3("Position", &position[0], 0.1f, -10.0f, 10.0f))
            {
                light.SetPosition(position);
            }

            // Intensity Control
            float intensity = light.GetIntensity();
            if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 10.0f))
            {
                light.SetIntensity(intensity);
            }
            break;
        }
    }

    ImGui::End();
}




void SceneManager::Terminate(GLFWwindow* window)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}


