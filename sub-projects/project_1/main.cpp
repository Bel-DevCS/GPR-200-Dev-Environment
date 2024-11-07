#include <iostream>
#include "assets/Code/Scene Manager/SceneManager.h"
#include "assets/Code/User Input/UserInput.h"


#include "Bella/Definitions/drawShape.h"
#include "Bella/Mechanic/shader.h"
#include "Bella/Mechanic/Model/mesh.h"
#include "Bella/Mechanic/Model/model.h"
#include "Bella/Mechanic/Lighting/Light Class/light.h"


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
    Bella_GPR200::Lighting::Light dirLight = Bella_GPR200::Lighting::Light::CreateDirectional(
    glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(1.0f, 1.0f, 1.0f)
);

    // Create a Point Light
    Bella_GPR200::Lighting::Light pointLight = Bella_GPR200::Lighting::Light::CreatePoint(
        glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3(1.0f, 0.8f, 0.6f), 1.0f
    );

    // Create a Spotlight
    Bella_GPR200::Lighting::Light spotLight = Bella_GPR200::Lighting::Light::CreateSpotlight(
        glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3(0.0f, -1.0f, 0.0f), 12.5f, glm::vec3(1.0f, 0.8f, 0.6f), 1.0f
    );


    //Initilize Shaders
    Bella_GPR200::Shader ourShader("assets/vertexShader.vert", "assets/fragmentShader.frag");
    Bella_GPR200::Shader genModelShader("assets/Shaders/Generic/genericModel.vert", "assets/Shaders/Generic/genericModel.frag");
    Bella_GPR200::Shader pixelShader("assets/Shaders/Pixel Shader/pixelVert.vert", "assets/Shaders/Pixel Shader/pixelFrag.frag");
   // Bella_GPR200::Shader testShader("assets/Shaders/Test Shaders/testShaderVertex.vert", "assets/Shaders/Test Shaders/testShaderFragment.frag");

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
        pointLight.SetLightUniforms(genModelShader);

        //Model Transformation
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.5f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        genModelShader.setMat4("model", model);

        //Draw the model
        testModel.Draw(genModelShader);

        //Draw UI
        SM.LightWindow(pointLight);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //10 : Terminate Program
    SM.Terminate(window);
}