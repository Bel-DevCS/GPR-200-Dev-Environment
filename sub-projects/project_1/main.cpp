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


const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;


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
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
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


    //9 : Render Loop
    while (!glfwWindowShouldClose(window)) {

        //9(a) : Clear the Screen
        glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //9(b) : Instantiate Shader Uniforms
        float timeValue = glfwGetTime();
        ourShader.setFloat("uTime", timeValue);

        //9(c) : Use Shader and Bind Vertex Array to Shader
        ourShader.use();
        simpleMesh.Draw(ourShader);


        //9(e) : Swap Buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    //10 : Terminate Program
    printf("Shutting down...");
    glfwTerminate();

}
