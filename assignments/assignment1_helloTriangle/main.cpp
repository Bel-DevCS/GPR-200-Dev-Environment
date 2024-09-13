#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

//Create Shape
float vertices[] =
        {
        //        X     Y     Z         R     G     B      A
                -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f, 1.0f,
                0.0f,  0.5f, 0.0f,      0.0f, 0.0f, 1.0f, 1.0f
        };

    #pragma region Shader Sources
const char *vertexShaderSource =
        R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec4 aColour;

            out vec4 Colour; //Varying

            void main()
            {
               Colour = aColour; //Pass-through
               gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
            };
        )";

const char *fragmentShaderSource =
        R"(
            #version 330 core
            out vec4 FragColor;

            in vec4 Colour;

            uniform float uTime = 1.0;

            void main()
            {
                FragColor = Colour; //vec4(1.0f, 0.5f, 0.2f, 1.0f);
            }
        )";

#pragma endregion

int main() {
    #pragma region Initialize Window
    printf("Initializing...");
    if (!glfwInit()) {
        printf("GLFW failed to init!");
        return 1;
    }
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

    #pragma region Create Vertex Array Object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

#pragma endregion

    #pragma region Create Vertex Buffer Object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //2
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //3
    //Dynamic Draw is for every once in a while manipulation, Stream Draw is for every frame

    //position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0); //4
    glEnableVertexAttribArray(0);

    //Colour (R,G,B,A)
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // glBindBuffer(GL_ARRAY_BUFFER, 0); //clears buffer
    //glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);
#pragma endregion

    #pragma region Vertex Shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);


    int  success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

#pragma endregion

    #pragma region Fragment Shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
#pragma endregion

    #pragma region Shader Program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success2;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success2);

    if(!success2)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
#pragma endregion


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    //Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

        //Input

        //Update
        float time = (float)glfwGetTime();

		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        //Set Time Unit
        int timeLoc = glGetUniformLocation(shaderProgram, "uTime");
        glUniform1f(timeLoc, time);
        glBindVertexArray(VAO);

        //Draw Call
        glDrawArrays(GL_TRIANGLES, 0, 3); //todo : figure out what this does

        //Drawing happens here!
		glfwSwapBuffers(window);

	}
	printf("Shutting down...");

}
