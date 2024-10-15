#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include <Bella/shader.h>
#include <Bella/definitionFunctions.h>
#include <Bella/texture.h>
#include <Bella/drawShape.h>
#include <Bella/definitionColours.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>


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
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3D Transforms and Triangles", NULL, NULL);
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

    Bella_GPR200::Texture2D aText("assets/Images/Example_Images/amythest.png");

    unsigned int cubeVAO = Bella_GPR200::DrawShape::Cube();

    glm::vec3 cubePositions[] =
            {
                glm::vec3(0.0f,  0.0f,  0.0f),
                glm::vec3(2.0f,  5.0f, -15.0f),
                glm::vec3(-1.5f, -2.2f, -2.5f),
                glm::vec3(-3.8f, -2.0f, -12.3f),
                glm::vec3(2.4f, -0.4f, -3.5f),
                glm::vec3(-1.7f,  3.0f, -7.5f),
                glm::vec3(1.3f, -2.0f, -2.5f),
                glm::vec3(1.5f,  2.0f, -2.5f),
                glm::vec3(1.5f,  0.2f, -1.5f),
                glm::vec3(-1.3f,  1.0f, -1.5f)
            };


    float rotationSpeeds[] = { 20.0f, 30.0f, 40.0f, 50.0f, 25.0f, 35.0f, 15.0f, 45.0f, 60.0f, 10.0f };


    glm::vec3 rotationAxes[] = {
            glm::vec3(1.0f, 0.3f, 0.5f),
            glm::vec3(0.5f, 1.0f, 0.3f),
            glm::vec3(0.3f, 0.5f, 1.0f),
            glm::vec3(1.0f, 0.7f, 0.2f),
            glm::vec3(0.2f, 1.0f, 0.8f),
            glm::vec3(0.6f, 0.4f, 1.0f),
            glm::vec3(0.3f, 1.0f, 0.5f),
            glm::vec3(1.0f, 0.6f, 0.3f),
            glm::vec3(0.8f, 0.3f, 1.0f),
            glm::vec3(1.0f, 0.2f, 0.7f)
    };

    float scales[] = { 1.0f, 1.2f, 0.8f, 1.1f, 0.9f, 1.3f, 1.0f, 0.7f, 1.4f, 0.6f };

    //glm::vec4* cubeColors = Colour::RandomColour(10);
    glm::vec4* cubeColours = Bella_GPR200::Colour::RandomColour(10);


    // Matrices (projection and view)
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));


    //9 : Render Loop
    while (!glfwWindowShouldClose(window)) {

        glEnable(GL_DEPTH_TEST);

        //Calculate Delta Time
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame;

        //9(a) : Clear the Screen
        glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //9(c) : Use Shader and Bind Vertex Array to Shader
        aText.Bind(0);
        ourShader.use();


        //Set Matrices
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        //Set Time
        ourShader.setFloat("uTime", currentFrame);

        //Bind
        glBindVertexArray(cubeVAO);

        for (unsigned int i = 0; i < 10; i++) {
            // Create model matrix for each cube
            glm::mat4 model = glm::mat4(1.0f);

            // Translate cube to its position
            model = glm::translate(model, cubePositions[i]);

            // Rotate cube over time
            float angle = rotationSpeeds[i] * currentFrame;
            model = glm::rotate(model, glm::radians(angle), rotationAxes[i]);

            //Scale :3
            model = glm::scale(model, glm::vec3(scales[i]));

            // Send the model matrix to the shader
            ourShader.setMat4("model", model);

            //Set Colour
            ourShader.setVec4("cubeColour", cubeColours[i]);

            // Draw the cube
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }


        //9(e) : Swap Buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    //10 : Terminate Program
    delete[] cubeColours;
    printf("Shutting down...");
    glfwTerminate();

}
