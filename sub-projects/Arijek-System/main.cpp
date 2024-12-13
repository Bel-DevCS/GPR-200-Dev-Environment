#include <iostream>
#include "assets/Code/Scene Manager/SceneManager.h"
#include "Bella/Mechanic/Terrain/TerrainGenerator.h"
#include "assets/Code/User Input/UserInput.h"

#include "ak/Camera.h"


#include "Bella/Definitions/Shapes/Vertex/drawShape.h"
#include "Bella/Mechanic/Shader/shader.h"
#include "Bella/Mechanic/Model/mesh.h"
#include "Bella/Mechanic/Model/model.h"
#include "Bella/Mechanic/Lighting/Light Class/light.h"
#include "Bella/Mechanic/Particle/ParticleSystem/ParticleSystem.h"


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
    GLFWwindow* window = SM.InitializeWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Arijek System");
    if (!window) return -1;
    SM.InitImGui(window);

    // OpenGL Settings
    glEnable(GL_DEPTH_TEST);

    // Set Up Camera Instances
    ak::Camera cam(glm::vec3(0.0f, 0.0f, 1.0f));
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

    //Particles
    Bella_GPR200::Shader snowShader("assets/Shaders/Particles/Snow/SnowVert.vert", "assets/Shaders/Particles/Snow/SnowFrag.frag");

    // Initialize Models
     Bella_GPR200::Model testModel("assets/Models/plant.fbx", genModelShader);

    //Terrain
    TerrainGenerator terrain(10, 5.0f, 23);

    //Particle System
    Bella_GPR200::ParticleSystem snowSystem(3000, snowShader);

    Bella_GPR200::ParticleConfig snowConfig;
    snowConfig.velocityMin = glm::vec3(-0.1f, -0.5f, -0.1f);
    snowConfig.velocityMax = glm::vec3(0.1f, -0.5f, 0.1f);
    snowConfig.colorStart = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    snowConfig.colorEnd = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
    snowConfig.lifespanMin = 15.0f;
    snowConfig.lifespanMax = 30.0f;
    snowConfig.sizeMin = 0.05f;
    snowConfig.sizeMax = 0.1f;

    snowSystem.SetConfig(snowConfig);
    snowSystem.setPosition(glm::vec3(0.0f, 10.0f, 0.0f));

    float terrainWidth = terrain.GetGridSize() * terrain.GetScale();
    float terrainDepth = terrain.GetGridSize() * terrain.GetScale();


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

        terrain.Render(cam, view, projection, SCREEN_WIDTH, SCREEN_HEIGHT);

        snowSystem.Update(deltaTime, terrainWidth, terrainDepth);
        snowSystem.Render(view, projection);


        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate Program
    SM.Terminate(window);
}