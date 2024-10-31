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
    Bella_GPR200::Shader lightCubeShader("assets/Shaders/Light Cube/lightCubeVert.vert", "assets/Shaders/Light Cube/lightCubeFrag.frag");

    Bella_GPR200::Shader ourShader("assets/Shaders/vertexShader.vert", "assets/Shaders/fragmentShader.frag");
    Bella_GPR200::Texture2D aText("assets/Textures/amethyst.png");

    unsigned int cubeVAO = Bella_GPR200::DrawShape::Cube();
    glm::vec3 cubePositions[numCubes];
    glm::vec3 cubeScales[numCubes];
    glm::vec3 cubeRotationAxes[numCubes];
    float cubeRotationSpeeds[numCubes];

    for (int i = 0; i < numCubes; ++i) {
        cubePositions[i] = glm::vec3(ew::RandomRange(-5.0f, 5.0f), ew::RandomRange(-5.0f, 5.0f), ew::RandomRange(-5.0f, 5.0f));
        cubeScales[i] = glm::vec3(ew::RandomRange(0.5f, 2.0f));
        cubeRotationAxes[i] = glm::normalize(glm::vec3(ew::RandomRange(0.0f, 1.0f), ew::RandomRange(0.0f, 1.0f), ew::RandomRange(0.0f, 1.0f)));
        cubeRotationSpeeds[i] = ew::RandomRange(1.0f, 5.0f);
    }
    glm::vec4* cubeColours = Bella_GPR200::Colour::RandomColour(numCubes);

    while (!glfwWindowShouldClose(window)) {
        glEnable(GL_DEPTH_TEST);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        UI.processInput(window, cam, deltaTime);

        glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = UI.getProjectionMatrix(cam, static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT);
        glm::mat4 view = cam.GetViewMatrix();

        // Main shader setup with lighting uniforms
        ourShader.use();
        aText.Bind(0);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);
        ourShader.setVec3("lightPos", pointLight.GetPosition());
        ourShader.setVec3("lightColor", pointLight.GetColor());
        ourShader.setFloat("lightIntensity", pointLight.GetIntensity());
        ourShader.setInt("lightType", 1);
        pointLight.SetLightUniforms(ourShader);

        // Draw each scene cube
        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < numCubes; i++) {
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
        lightCubePos.y += 1.0f; // Offset by +1.0f on Y

        glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), lightCubePos);
        lightCubeShader.setMat4("model", lightModel);

        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete[] cubeColours;
    printf("Shutting down...");
    SM.Terminate(window);

    return 0;
}
