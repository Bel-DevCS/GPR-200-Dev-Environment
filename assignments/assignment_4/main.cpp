#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Bella/Mechanic/shader.h"
#include "Bella/Mechanic/texture.h"
#include "Bella/Mechanic/camera.h"

#include "Bella/Definitions/drawShape.h"
#include "Bella/Definitions/definitionFunctions.h"
#include "Bella/Definitions/definitionColours.h"



const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float deltaTime = 0.0f;  // Time between current frame and last frame
float lastFrame = 0.0f;  // Time of the last frame

//Global Variable for Number of cubes being drawn
int numCubes = 20;

//Global Camera object to handle inputs later
Bella_GPR200::Camera cam(glm::vec3(0.0f, 0.0f, 1.0f));

//Global Camera Variables
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;
bool isPerspective = true;
bool isMouseLocked = true;


//Callback functions
//! : potentially add to camera class?
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

float bounce = 0.5;

int main() {

    //1 : Create Program Window
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
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3D Transforms and Triangles", nullptr, nullptr);
    if (window == nullptr) {
        printf("GLFW failed to create window");
        return 1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGL(glfwGetProcAddress)) {
        printf("GLAD Failed to load GL headers");
        return 1;
    }

    //1(d) : Register Callbacks
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    //! : potentially set as global Variables?
    // 2 : Instantiate Shader and Load Textures
    Bella_GPR200::Shader ourShader("assets/Shaders/vertexShader.vert", "assets/Shaders/fragmentShader.frag");
    Bella_GPR200::Texture2D aText("assets/Textures/amethyst.png");

    // 3 : Instantiate Cube and Attributes
    unsigned int cubeVAO = Bella_GPR200::DrawShape::Cube();
    glm::vec3 cubePositions[numCubes];
    glm::vec3 cubeScales[numCubes];
    glm::vec3 cubeRotationAxes[numCubes];
    float cubeRotationSpeeds[numCubes];


    //4 : Define Cube Attributes
    for (int i = 0; i < numCubes; ++i)
    {
        cubePositions[i] =
                glm::vec3
                (
                ew::RandomRange(-5.0f, 5.0f),
                ew::RandomRange(-5.0f, 5.0f),
                ew::RandomRange(-5.0f, 5.0f)
                );

        cubeScales[i] = glm::vec3(ew::RandomRange(0.5f, 2.0f));

        cubeRotationAxes[i] = glm::normalize(glm::vec3(
                ew::RandomRange(0.0f, 1.0f),
                ew::RandomRange(0.0f, 1.0f),
                ew::RandomRange(0.0f, 1.0f)));
        cubeRotationSpeeds[i] = ew::RandomRange(1.0f, 5.0f);

    }

    glm::vec4* cubeColours = Bella_GPR200::Colour::RandomColour(numCubes);


    // 5 : Set Projection Matrix
    glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);


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

        cam.MovementSpeed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
                || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) ? 10.0f : 1.0f;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cam.KeyboardInput(Bella_GPR200::FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cam.KeyboardInput(Bella_GPR200::BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cam.KeyboardInput(Bella_GPR200::LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cam.KeyboardInput(Bella_GPR200::RIGHT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) cam.KeyboardInput(Bella_GPR200::DOWN, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) cam.KeyboardInput(Bella_GPR200::UP, deltaTime);

        // 6(d) : Handle Projection Toggle
        static bool spacePressed = false;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !spacePressed) {
            isPerspective = !isPerspective;  // Toggle projection mode
            spacePressed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
        {
            spacePressed = false;
        }

        // 6(e) : Lock and Unlock Mouse
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            isMouseLocked = !isMouseLocked;
            glfwSetInputMode(window, GLFW_CURSOR, isMouseLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
            glfwWaitEventsTimeout(0.2);
        }

        // 6(f) : Update Projection Matrix
        projection = isPerspective
                     ? glm::perspective(glm::radians(cam.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f)
                     : glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 1000.0f);


        // 6(g) : Use Shader, Set Matrices
        aText.Bind(0);
        ourShader.use();

        glm::mat4 view = cam.GetViewMatrix();
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);


        // 6(h) Set time uniform
        ourShader.setFloat("uTime", currentFrame);
        ourShader.setFloat("uBounce", bounce);

        // 6(i) : Draw Each Cube
        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < numCubes; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, cubeRotationSpeeds[i] * currentFrame, cubeRotationAxes[i]);
            model = glm::scale(model, cubeScales[i]);

            ourShader.setMat4("model", model);
            ourShader.setVec4("cubeColour", cubeColours[i]);
            ourShader.setFloat("uOscillationOffset", i * 0.5f);

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        }


        // 6(j) : Swap Buffers and Poll Events
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    //7 : Terminate Program
    delete[] cubeColours;
    printf("Shutting down...");
    glfwTerminate();

}

//todo : check and see if there is a way to modify this into the camera class
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
    cam.ScrollInput(static_cast<float>(y_offset));
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    if (isMouseLocked)
    {
        cam.MouseInput(xoffset, yoffset);
    }
}