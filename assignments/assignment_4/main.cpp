#include <ew/external/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include <Bella/shader.h>
#include <Bella/definitionFunctions.h>
#include <Bella/texture.h>
#include <Bella/drawShape.h>
#include <Bella/definitionColours.h>
#include <Bella/camera.h>


const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float deltaTime = 0.0f;  // Time between current frame and last frame
float lastFrame = 0.0f;  // Time of the last frame

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

    // 2 : Set Up Camera
    Bella_GPR200::Camera cam(glm::vec3(0.0f, 0.0f, 1.0f));
    float normalSpeed = 1.0f;
    float shiftSpeed = 10.0f;


    // 3 : Instantiate Shader and Load Textures
    Bella_GPR200::Shader ourShader("assets/vertexShader.vert", "assets/fragmentShader.frag");
    Bella_GPR200::Texture2D aText("assets/Images/Example_Images/amythest.png");

    // 4 : Setup Cube Positions and Attributes
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


    float rotationSpeeds[] =
            {
                20.0f,
                30.0f,
                40.0f,
                50.0f,
                25.0f,
                35.0f,
                15.0f,
                45.0f,
                60.0f,
                10.0f
            };


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

    float scales[] =
            {
                1.0f,
                1.2f,
                0.8f,
                1.1f,
                0.9f,
                1.3f,
                1.0f,
                0.7f,
                1.4f,
                0.6f
            };

    glm::vec4* cubeColours = Bella_GPR200::Colour::RandomColour(10);


    // 5 : Set Projection Matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);


    // 6 : Main Render Loop
    while (!glfwWindowShouldClose(window)) {

        glEnable(GL_DEPTH_TEST);

        // 6(a) : Calculate Delta Time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // 6(b) : Clear Screen
        glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 6(c) : Handle Camera Movements
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
        {
            cam.MovementSpeed = shiftSpeed;  // Increase speed when Shift is pressed
        }
        else
        {
            cam.MovementSpeed = normalSpeed;  // Default speed
        }

        // Camera movement input handling
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cam.KeyboardInput(Bella_GPR200::FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cam.KeyboardInput(Bella_GPR200::BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cam.KeyboardInput(Bella_GPR200::LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cam.KeyboardInput(Bella_GPR200::RIGHT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            cam.KeyboardInput(Bella_GPR200::UP, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            cam.KeyboardInput(Bella_GPR200::DOWN, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            cam.KeyboardInput(Bella_GPR200::ROTATE_LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            cam.KeyboardInput(Bella_GPR200::ROTATE_RIGHT, deltaTime);



        // 6(d) : Use Shader, Set Matrices
        aText.Bind(0);
        ourShader.use();

        glm::mat4 view = cam.GetViewMatrix();
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);


        // Set time uniform
        ourShader.setFloat("uTime", currentFrame);

        // 6(e) : Draw Each Cube
        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = rotationSpeeds[i] * currentFrame;
            model = glm::rotate(model, glm::radians(angle), rotationAxes[i]);
            model = glm::scale(model, glm::vec3(scales[i]));

            ourShader.setMat4("model", model);
            ourShader.setVec4("cubeColour", cubeColours[i]);
            ourShader.setFloat("uOscillationOffset", i * 0.5f);

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }


        // 6(f) : Swap Buffers and Poll Events
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    //7 : Terminate Program
    delete[] cubeColours;
    printf("Shutting down...");
    glfwTerminate();

}
