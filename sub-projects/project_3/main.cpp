#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

#include "Bella/Mechanic/Shader/shader.h"
#include "assets/code/SceneManager/SceneManager.h"
#include "Bella/Mechanic/Camera/camera.h"
#include "Bella/Mechanic/Model/model.h"


const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float deltaTime = 0.0f;  // Time between current frame and last frame
float lastFrame = 0.0f;  // Time of the last frame

SceneManager SM;

int main() {

    //1 : Innit Program Window
    GLFWwindow* window = SM.InitializeWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project 3 : AAAAA");
    if (!window) return -1;
    SM.InitImGui(window);

    //2 : Innit Shader(s)
    Bella_GPR200::Shader ourShader("assets/vertexShader.vert", "assets/fragmentShader.frag");
    Bella_GPR200::Shader genModelShader("assets/shaders/generic/genericModel.vert",
                                        "assets/shaders/generic/genericModel.frag");


    //3 : Innit Camera
    Bella_GPR200::Camera cam(glm::vec3(0.0f, 0.0f, 1.0f));

    //4 : Innit Light
    Bella_GPR200::Lighting::Light pointLight = Bella_GPR200::Lighting::Light::CreatePoint(
            glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3(1.0f, 0.8f, 0.6f), 1.0f
    );


    //5 : Innit Model
    Bella_GPR200::Model lamp("assets/models/lamp.zip");



    //9 : Render Loop
    while (!glfwWindowShouldClose(window)) {

        //Time Calculations
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

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
       // pointLight.SetLightUniforms(genModelShader);

        //Model Transformation
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.5f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        genModelShader.setMat4("model", model);

        //Draw the model
        lamp.Draw(genModelShader);

        //Draw UI
        SM.LightWindow(pointLight);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //10 : Terminate Program
     SM.Terminate(window);

}
