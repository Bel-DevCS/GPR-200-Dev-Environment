#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

#include "Bella/Definitions/drawShape.h"
#include "Bella/Mechanic/shader.h"
#include "Bella/Mechanic/Model/mesh.h"
#include "Bella/Mechanic/Model/model.h"
#include "Bella/Mechanic/camera.h"


const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float deltaTime = 0.0f;  // Time between current frame and last frame
float lastFrame = 0.0f;  // Time of the last frame
Bella_GPR200::Camera cam(glm::vec3(0.0f, 0.0f, 1.0f));

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void processInput(GLFWwindow* window, Bella_GPR200::Camera& camera, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.KeyboardInput(Bella_GPR200::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.KeyboardInput(Bella_GPR200::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.KeyboardInput(Bella_GPR200::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.KeyboardInput(Bella_GPR200::RIGHT, deltaTime);
}

int pixelSize = 200;

glm::vec3 lightDir = glm::vec3(-0.2f, -1.0f, -0.3f); // Example direction
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);  // White light




int main() {

    //1 : Create Program Window
    #pragma region Initialize Window
    printf("Initializing...");

    //1(a) : Ensure Core Profile is Correct
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //1(b) : Check if GLFW fails to Initiate
    if (!glfwInit()) {
        printf("GLFW failed to init!");
        return 1;
    }

    //1(c) : Create Window
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project 1 : Models", NULL, NULL);
    if (window == NULL) {
        printf("GLFW failed to create window");
        return 1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGL(glfwGetProcAddress)) {
        printf("GLAD Failed to load GL headers");
        return 1;
    }
#pragma endregion

    //2 : Instantiate Shader
    Bella_GPR200::Shader ourShader("assets/vertexShader.vert", "assets/fragmentShader.frag");

    Bella_GPR200::Shader genModelShader("assets/Shaders/Generic/genericModel.vert", "assets/Shaders/Generic/genericModel.frag");
    Bella_GPR200::Shader pixelShader("assets/Shaders/Pixel Shader/pixelVert.vert", "assets/Shaders/Pixel Shader/pixelFrag.frag");

    //Mesh Testing

    std::vector<Bella_GPR200::Vertex> vertices =
        {
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // Bottom left
            {{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}, // Bottom right
            {{0.0f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.5f, 1.0f}} // Top center
        };

    std::vector<unsigned int> indices = {0, 1, 2};
    std::vector<Bella_GPR200::Texture> textures;

    //4 : Create a simple Mesh
    Bella_GPR200::Mesh simpleMesh(vertices, indices, textures);

    Bella_GPR200::Model testModel("assets/Models/plant.fbx");
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);


    //9 : Render Loop
    // Render Loop
    while (!glfwWindowShouldClose(window)) {

        // Calculate delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process input
        processInput(window, cam, deltaTime);



        // Clear color and depth buffers
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update projection and view matrices
        glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = cam.GetViewMatrix();

        // Set shader uniforms
        genModelShader.use();
        genModelShader.setMat4("projection", projection);
        genModelShader.setMat4("view", view);
        genModelShader.setVec3("lightDir", lightDir);
        genModelShader.setVec3("lightColor", lightColor);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        genModelShader.setMat4("model", model);
        // Draw the model
        testModel.Draw(genModelShader);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    //10 : Terminate Program
    printf("Shutting down...");
    glfwTerminate();

}