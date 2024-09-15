#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

#include <Bella/shader.h>


const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;


int main() {

    #pragma region Initialize Window
    printf("Initializing...");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


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

    Bella_GPR200::Shader ourShader("assets/vertexShader.vert", "assets/fragmentShader.frag");

    float vertices[] =
            {
                    //        X     Y     Z         R     G     B      A
                    -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f, 1.0f,
                    0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f, 1.0f,
                    0.0f,  0.5f, 0.0f,      0.0f, 0.0f, 1.0f, 1.0f
            };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);




// In your render loop:
    while (!glfwWindowShouldClose(window)) {
        // Clear the screen
        glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //Time
        float timeValue = glfwGetTime();  // Get the current time
        ourShader.setFloat("uTime", timeValue);

        //triangle
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);


        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    printf("Shutting down...");
    glfwTerminate();

}
