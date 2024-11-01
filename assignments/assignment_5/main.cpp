#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "scripts/Scene Manager/SceneManager.h"
#include "scripts/User Input/UserInput.h"
#include "Bella/Mechanic/shader.h"
#include "Bella/Mechanic/texture.h"
#include "Bella/Definitions/drawShape.h"
#include "Bella/Definitions/definitionFunctions.h"
#include "Bella/Definitions/definitionColours.h"

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
int numCubes = 20;
float bounce = 0.5f;

Bella_GPR200::Camera cam(glm::vec3(0.0f, 0.0f, 1.0f));
SceneManager SM;
UserInput UI;

void SimpleLightControlUI(Bella_GPR200::Lighting::Light light);
void SimpleButton();


int main() {
    InputContext inputContext = {&cam, &UI};
    GLFWwindow* window = SceneManager::InitializeWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Assignment 5 : Lighting");
    if (!window) return -1;
    SM.InitImGui(window);

    glfwSetWindowUserPointer(window, &inputContext);
    glfwSetScrollCallback(window, UserInput::scroll_callback);
    glfwSetCursorPosCallback(window, UserInput::mouse_callback);

    Bella_GPR200::Lighting::Light pointLight = Bella_GPR200::Lighting::Light::CreatePoint(
        glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3(1.0f, 0.8f, 0.6f), 5.0f);

    pointLight.SetLightingModel(Bella_GPR200::Lighting::LightingModel::BLINN_PHONG);

    Bella_GPR200::Shader lightCubeShader("assets/Shaders/Light Cube/lightCubeVert.vert", "assets/Shaders/Light Cube/lightCubeFrag.frag");

    Bella_GPR200::Shader ourShader("assets/Shaders/vertexShader.vert", "assets/Shaders/fragmentShader.frag");
    Bella_GPR200::Texture2D aText("assets/Textures/amethyst.png");

    //Create Cube
    unsigned int cubeVAO = Bella_GPR200::DrawShape::Cube();
    glm::vec3 cubePositions[numCubes];
    glm::vec3 cubeScales[numCubes];
    glm::vec3 cubeRotationAxes[numCubes];
    float cubeRotationSpeeds[numCubes];

    for (int i = 0; i < numCubes; ++i)
    {
        cubePositions[i] = glm::vec3(ew::RandomRange(-5.0f, 5.0f), ew::RandomRange(-5.0f, 5.0f), ew::RandomRange(-5.0f, 5.0f));
        cubeScales[i] = glm::vec3(ew::RandomRange(0.5f, 2.0f));
        cubeRotationAxes[i] = glm::normalize(glm::vec3(ew::RandomRange(0.0f, 1.0f), ew::RandomRange(0.0f, 1.0f), ew::RandomRange(0.0f, 1.0f)));
        cubeRotationSpeeds[i] = ew::RandomRange(1.0f, 5.0f);
    }

    glm::vec4* cubeColours = Bella_GPR200::Colour::RandomColour(numCubes);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureMouse) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            UI.isMouseLocked = false;
        } else if (UI.isMouseLocked) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }



        glfwPollEvents();

        //Time Calculation
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        //Process Input
        UI.processInput(window, cam, deltaTime);

        // Clear color and depth buffers
        glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Main shader setup with lighting uniforms
        ourShader.use();
        aText.Bind(0);
        glm::mat4 projection = UI.getProjectionMatrix(cam, static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT);
        glm::mat4 view = cam.GetViewMatrix();
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        pointLight.SetLightUniforms(ourShader);


        // Draw each scene cube
        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < numCubes; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, cubeRotationSpeeds[i] * currentFrame, cubeRotationAxes[i]);
            model = glm::scale(model, cubeScales[i]);

            ourShader.setMat4("model", model);
            ourShader.setVec4("cubeColour", cubeColours[i]);
            ourShader.setFloat("uOscillationOffset", i * 0.5f);

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        }
        glBindVertexArray(0);

        // Draw the light cube, offset by +1.0f on Y
        lightCubeShader.use();
        lightCubeShader.setMat4("view", view);
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setVec3("cubeColor", pointLight.GetColor());

        glm::vec3 lightCubePos = pointLight.GetPosition();
        lightCubePos.y += 1.0f;

        glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), lightCubePos);
        lightCubeShader.setMat4("model", lightModel);

        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        //Draw UI
       SM.LightWindow(pointLight);
       //SimpleLightControlUI(pointLight);
       // SimpleButton();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(window);
    }

    delete[] cubeColours;
    printf("Shutting down...");
    SM.Terminate(window);

    return 0;
}


void SimpleLightControlUI(Bella_GPR200::Lighting::Light light) {
    // Start a new ImGui window for light control
    ImGui::Begin("Light Settings");

    // Retrieve current K values from the light
    float ambientK = light.GetAmbientK();
    float diffuseK = light.GetDiffuseK();
    float specularK = light.GetSpecularK();
    glm::vec3 position = light.GetPosition();

    // ImGui sliders for K values
    ImGui::Text("Light Intensity Settings");
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

    ImGui::Spacing();
    ImGui::Text("Light Position");
    ImGui::Separator();

    // Light position controls
    if (ImGui::DragFloat3("Position", &position[0], 0.1f, -10.0f, 10.0f)) {
        light.SetPosition(position);
    }

    ImGuiIO& io = ImGui::GetIO();
    std::cout << "WantCaptureMouse: " << io.WantCaptureMouse << std::endl;

    ImGui::End(); // End of Light Settings window
}

void SimpleButton()
{
    ImGui::Begin("Simple Button Window"); // Start a new window
    if (ImGui::Button("Click Me")) { // Create a button
        std::cout << "Button clicked!" << std::endl; // Print message on click
    }
    ImGui::End(); // End the window
}
