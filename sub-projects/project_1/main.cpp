#include <iostream>
#include "assets/Code/Scene Manager/SceneManager.h"
#include "assets/Code/User Input/UserInput.h"


#include "Bella/Definitions/drawShape.h"
#include "Bella/Mechanic/shader.h"
#include "Bella/Mechanic/Model/mesh.h"
#include "Bella/Mechanic/Model/model.h"
#include "Bella/Mechanic/light.h"


//Global Variables
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;
float deltaTime = 0.0f;  // Time between current frame and last frame
float lastFrame = 0.0f;  // Time of the last frame

//Managers
SceneManager SM;
UserInput UI;

int main() {

    //Create OpenGL Window and ImGui window
    GLFWwindow* window = SM.InitializeWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project 1 : Models");
    if (!window) return -1;
    SM.InitImGui(window);

    //Set Up Camera
    Bella_GPR200::Camera cam(glm::vec3(0.0f, 0.0f, 1.0f));

    //Set Up Lighting
    Bella_GPR200::Lighting::Light directionalLight(glm::vec3(-0.2f, 1.0f, -0.3f), glm::vec3(1.0f, 1.0f, 1.0f));
    directionalLight.SetLightingModel(Bella_GPR200::Lighting::LightingModel::PHONG);

    //Initilize Shaders
    Bella_GPR200::Shader ourShader("assets/vertexShader.vert", "assets/fragmentShader.frag");
    Bella_GPR200::Shader genModelShader("assets/Shaders/Generic/genericModel.vert", "assets/Shaders/Generic/genericModel.frag");
    Bella_GPR200::Shader pixelShader("assets/Shaders/Pixel Shader/pixelVert.vert", "assets/Shaders/Pixel Shader/pixelFrag.frag");

    //Initilize Models
    Bella_GPR200::Model testModel("assets/Models/plant.fbx");

    //OpenGL Settings
    glEnable(GL_DEPTH_TEST);

    //Render Loop
    while (!glfwWindowShouldClose(window)) {

        //Time Calculations
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process input
        UI.processInput(window, cam, deltaTime);

        // Clear color and depth buffers
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Shader Instantiation
        genModelShader.use();

        // Update projection and view matrices
        glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = cam.GetViewMatrix();
        genModelShader.setMat4("projection", projection);
        genModelShader.setMat4("view", view);

        //Lighting Instantiation
        directionalLight.SetLightUniforms(genModelShader);

        //Model Transformation
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        genModelShader.setMat4("model", model);

        //Draw the model
        testModel.Draw(genModelShader);

        //Draw UI
        SM.LightWindow(directionalLight);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //10 : Terminate Program
    SM.Terminate(window);
}