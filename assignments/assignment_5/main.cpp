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

float deltaTime = 0.0f;  // Time between current frame and last frame
float lastFrame = 0.0f;  // Time of the last frame

//Global Variable for Number of cubes being drawn
int numCubes = 20;

//Global Camera object to handle inputs later
Bella_GPR200::Camera cam(glm::vec3(0.0f, 0.0f, 1.0f));

SceneManager SM;
UserInput UI;


float bounce = 0.5;

int main() {

    InputContext inputContext = {&cam, &UI};

    GLFWwindow* window = SceneManager::InitializeWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Assignment 5 : Lighting");
    if (!window) return -1;
    SM.InitImGui(window);

    // Set the window user pointer to access context in callbacks
    glfwSetWindowUserPointer(window, &inputContext);
    glfwSetScrollCallback(window, UserInput::scroll_callback);
    glfwSetCursorPosCallback(window, UserInput::mouse_callback);


    //! : potentially set as global Variables?
    // 2 : Instantiate Shader and Load Textures
    Bella_GPR200::Shader ourShader("assets/Shaders/vertexShader.vert", "assets/Shaders/fragmentShader.frag");
    Bella_GPR200::Texture2D aText("assets/Textures/amethyst.png");

    // 3 : Instantiate Cube and Attributes
    unsigned int cubeVAO = Bella_GPR200::DrawShape::Cube();
    glm::vec3 cubePositions[numCubes];
    glm::vec3 cubeScales[numCubes];
    glm::vec3 cubeRotationAxes[numCubes];
    float cubeRotationSpeeds[numCubes];


    //4 : Define Cube Attributes
    for (int i = 0; i < numCubes; ++i)
    {
        cubePositions[i] =
                glm::vec3
                (
                ew::RandomRange(-5.0f, 5.0f),
                ew::RandomRange(-5.0f, 5.0f),
                ew::RandomRange(-5.0f, 5.0f)
                );

        cubeScales[i] = glm::vec3(ew::RandomRange(0.5f, 2.0f));

        cubeRotationAxes[i] = glm::normalize(glm::vec3(
                ew::RandomRange(0.0f, 1.0f),
                ew::RandomRange(0.0f, 1.0f),
                ew::RandomRange(0.0f, 1.0f)));
        cubeRotationSpeeds[i] = ew::RandomRange(1.0f, 5.0f);

    }

    glm::vec4* cubeColours = Bella_GPR200::Colour::RandomColour(numCubes);


    // 5 : Set Projection Matrix
    glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);


    // 6 : Main Render Loop
    while (!glfwWindowShouldClose(window)) {

        glEnable(GL_DEPTH_TEST);

        // Calculate Delta Time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        UI.processInput(window, cam, deltaTime);

        // Clear Screen
        glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update Projection Matrix based on perspective toggle
        glm::mat4 projection = UI.getProjectionMatrix(cam, static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT);

        // Use Shader and Set Matrices
        aText.Bind(0);
        ourShader.use();

        glm::mat4 view = cam.GetViewMatrix();
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        // Set time uniform
        ourShader.setFloat("uTime", currentFrame);
        ourShader.setFloat("uBounce", bounce);

        // Draw Each Cube
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

        // Swap Buffers and Poll Events
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    //7 : Terminate Program
    delete[] cubeColours;
    printf("Shutting down...");
    glfwTerminate();

}