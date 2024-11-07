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

// Screen dimensions
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

// Time tracking
float deltaTime = 0.0f;
float lastFrame = 0.0f;
int numCubes = 1;

Bella_GPR200::Camera cam(glm::vec3(0.0f, 0.0f, 1.0f));
SceneManager SM;
UserInput UI;

void SimpleLightControlUI(Bella_GPR200::Lighting::Light light);
void SimpleButton();

int main() {
    InputContext inputContext = {&cam, &UI};
    GLFWwindow* window = SceneManager::InitializeWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Assignment 5 : Lighting");
    if (!window) return -1;

    // Set up window context for input callbacks
    glfwSetWindowUserPointer(window, &inputContext);
    glfwSetScrollCallback(window, UserInput::scroll_callback);
    glfwSetCursorPosCallback(window, UserInput::mouse_callback);

    // Initialize ImGui
    SM.InitImGui(window);

    // Set up lighting
    Bella_GPR200::Lighting::Light pointLight = Bella_GPR200::Lighting::Light::CreatePoint(
            glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3(1.0f, 0.8f, 0.6f), 5.0f
    );
    pointLight.SetLightingModel(Bella_GPR200::Lighting::LightingModel::BLINN_PHONG);

    // Load shaders and textures
    Bella_GPR200::Shader ourShader("assets/Shaders/vertexShader.vert", "assets/Shaders/fragmentShader.frag");
    ourShader.AddLighting(pointLight);  // Add lighting parameters to the shader
    Bella_GPR200::Texture2D aText("assets/Textures/amethyst.png");

    // Create Cube VAO
    unsigned int cubeVAO = Bella_GPR200::DrawShape::Cube();

    // Set up cube transformations
    glm::vec3 cubeScales[numCubes];
    glm::vec3 cubeRotationAxes[numCubes];
    float cubeRotationSpeeds[numCubes];

    for (int i = 0; i < numCubes; ++i) {
        cubeScales[i] = glm::vec3(ew::RandomRange(0.5f, 2.0f));
        cubeRotationAxes[i] = glm::normalize(glm::vec3(ew::RandomRange(0.0f, 1.0f), ew::RandomRange(0.0f, 1.0f), ew::RandomRange(0.0f, 1.0f)));
        cubeRotationSpeeds[i] = ew::RandomRange(1.0f, 5.0f);
    }

    glm::vec4* cubeColours = Bella_GPR200::Colour::RandomColour(numCubes);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        // Start new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Input handling for ImGui and cursor
        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureMouse) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            UI.isMouseLocked = false;
        } else if (UI.isMouseLocked) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        // Poll events and calculate deltaTime
        glfwPollEvents();
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process user input
        UI.processInput(window, cam, deltaTime);

        // Clear buffers
        glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind shader and set up uniform variables
        ourShader.use();
        aText.Bind(0);

        // Projection and view matrices
        glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = cam.GetViewMatrix();
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        // Set lighting uniforms in the shader
        pointLight.SetLightUniforms(ourShader);

        // Draw cubes
        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < numCubes; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, cubeRotationSpeeds[i] * currentFrame, cubeRotationAxes[i]);
            model = glm::scale(model, cubeScales[i]);

            ourShader.setMat4("model", model);
            ourShader.setVec4("cubeColour", cubeColours[i]);
            ourShader.setFloat("uOscillationOffset", i * 0.5f);

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        }
        glBindVertexArray(0);

        // Render ImGui UI
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Cleanup
    delete[] cubeColours;
    printf("Shutting down...");
    SM.Terminate(window);

    return 0;
}
