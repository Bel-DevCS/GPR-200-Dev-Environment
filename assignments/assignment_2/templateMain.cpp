//
// Created by Bella Wunsch on 2024-09-26.
//

#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

#include <Bella/shader.h>
#include <Bella/texture.h>

#include <EW/external/stb_image.h>


const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;


void Template() {

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
    }

    //1(c) : Create Window
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Assignment 2 : Textures", NULL, NULL);
    if (window == NULL) {
        printf("GLFW failed to create window");
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGL(glfwGetProcAddress)) {
        printf("GLAD Failed to load GL headers");
    }
#pragma endregion

    //2 : Instantiate Shader
    Bella_GPR200::Shader ourShader("assets/vertexShader.vert", "assets/fragmentShader.frag");

    //3 : Initiate Texture(s)
    Bella_GPR200::Texture2D myTexture("assets/Images/ExampleImages/container.jpg");
    Bella_GPR200::Texture2D faceTexture("assets/Images/ExampleImages/awesomeface.png");

    //4(a) : Create Shape
    float vertices[] =
            {
                    //       X     Y     Z         R     G     B     A       S    T      R
                    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,     1.0f, 1.0f, 1.0f,  // Top right
                    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 0.0f, 1.0f,  // Bottom right
                    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,  // Bottom left
                    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,  // Top left

            };

    unsigned int indices[] =
            {
                    0, 1, 2,    //Triangle 1
                    0, 2, 3     //Triangle 2
            };

    //4(b) : Create Texture Coords
    float texCoords[] =
            {
                    0.0f, 0.0f,
                    1.0f, 0.0f,
                    0.0f, 1.0f,
                    1.0f, 1.0f
            };



    //glTexParameteri (Texture Target, Which Option for which axis [S,T,R == X,Y,Z], Texture Wrap Type)
    //TexParameteri -> Texture Parameter Integer
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    //glTexParameterfv(Texture Target, Action, Applied Filter / Action)



    //5 : Instantiate Vertex Array Object, and Vertex Buffer Object
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //6 : Initiate Element Buffer Object
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    //7 : Bind Vertex Array Object
    glBindVertexArray(VAO);

    //8 : Bind Vertex Array Object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //9 : Bind Element Buffer Object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //10 : Attribute Pointer for Position (X,Y,Z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //11 : Attribute Pointer for Colour (R,G,B,A)
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //12 : Attribute Pointer for Texture (S,T, R)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //13 : Render Loop
    while (!glfwWindowShouldClose(window)) {

        //13(a) : Clear the Screen
        glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //13(b) : Instantiate Shader Uniforms
        float timeValue = glfwGetTime();
        ourShader.setFloat("uTime", timeValue);


        //13(c) : Use Shader and Bind Vertex Array to Shader
        myTexture.Bind(0);
        faceTexture.Bind(1);
        ourShader.use();
        ourShader.setInt("texture1", 0);
        ourShader.setBool("texture2", 1);

        glBindVertexArray(VAO);

        //13(d) : Draw Call
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        //13(e) : Swap Buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    //14 : Terminate Program
    printf("Shutting down...");
    glfwTerminate();

}
