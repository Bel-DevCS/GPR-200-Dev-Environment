#include <iostream>
#include "assets/Code/Scene Manager/SceneManager.h"
#include "assets/Code/Terrain/TerrainGenerator.h"
#include "assets/Code/User Input/UserInput.h"


#include "Bella/Definitions/Shapes/Vertex/drawShape.h"
#include "Bella/Mechanic/Shader/shader.h"
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

//Ung
void DrawModel();


//debug Statics


int main() {

    //Create OpenGL Window and ImGui window
    GLFWwindow* window = SM.InitializeWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Arijek System");
    if (!window) return -1;
    SM.InitImGui(window);

    // OpenGL Settings
    glEnable(GL_DEPTH_TEST);

    // Set Up Camera Instances
    Bella_GPR200::Camera cam(glm::vec3(0.0f, 0.0f, 1.0f));
    Bella_GPR200::Camera isoCam(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 20.0f, true);


    // Set Up Lighting
     Bella_GPR200::Lighting::Light dirLight = Bella_GPR200::Lighting::Light::CreateDirectional(
        glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(1.0f, 1.0f, 1.0f)
    );

    // Create a Point Light
     Bella_GPR200::Lighting::Light pointLight = Bella_GPR200::Lighting::Light::CreatePoint(
        glm::vec3(10.0f, 1.0f, 2.0f), glm::vec3(1.0f, 0.8f, 0.6f), 1.0f
    );

    // Create a Spotlight
     Bella_GPR200::Lighting::Light spotLight = Bella_GPR200::Lighting::Light::CreateSpotlight(
        glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3(0.0f, -1.0f, 0.0f), 12.5f, glm::vec3(1.0f, 0.8f, 0.6f), 1.0f
    );

    // Initialize Shaders
     Bella_GPR200::Shader ourShader("assets/vertexShader.vert", "assets/fragmentShader.frag");
     Bella_GPR200::Shader genModelShader("assets/Shaders/Generic/genericModel.vert", "assets/Shaders/Generic/genericModel.frag");
     Bella_GPR200::Shader pixelShader("assets/Shaders/Pixel Shader/pixelVert.vert", "assets/Shaders/Pixel Shader/pixelFrag.frag");

    // Initialize Models
     Bella_GPR200::Model testModel("assets/Models/plant.fbx");

    //Terrain
    TerrainGenerator terrain(10, 5.0f, 7);

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)
        {
      glViewport(0, 0, width, height);
        });

    // Render Loop
    while (!glfwWindowShouldClose(window)) {

        // Time Calculations
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process input
        UI.processInput(window, cam, deltaTime);

        // Clear color and depth buffers
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Shader Instantiation
        genModelShader.use();

        // Update projection and view matrices
        glm::mat4 projection = cam.GetProjectionMatrix((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT);
        glm::mat4 view = cam.GetViewMatrix();
        genModelShader.setMat4("projection", projection);
        genModelShader.setMat4("view", view);

        // Lighting Instantiation
        pointLight.SetLightUniforms(genModelShader);

        terrain.Render(cam, SCREEN_WIDTH, SCREEN_HEIGHT);

        /*
        // Model Transformation
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
        genModelShader.setMat4("model", model);

        // Draw the model
        testModel.Draw(genModelShader);

        */

        // Draw UI
      //  SM.LightWindow(pointLight);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate Program
    SM.Terminate(window);
}