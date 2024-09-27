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
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Assignment 2 : Textures", NULL, NULL);
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

    // 2: Instantiate shaders
    Bella_GPR200::Shader BGShader(
            "assets/Shaders/Background Shader/BG_VertexShader.vert",
            "assets/Shaders/Background Shader/BG_FragmentShader.frag");
    Bella_GPR200::Shader characterShader(
            "assets/Shaders/Character Shader/Character_VertexShader.vert",
            "assets/Shaders/Character Shader/Character_FragmentShader.frag");

    // 3: Instantiate textures
    Bella_GPR200::Texture2D BGTexture("assets/Images/AssignmentImages/BackgroundImages/BG.jpg");
    Bella_GPR200::Texture2D characterTexture("assets/Images/AssignmentImages/CharacterImages/Character.png");

    // 4: Quad vertices
    float vertices[] = {
            // Positions            Colors                  Texture Coords
            0.5f,  0.5f, 0.0f,      1.0f, 0.0f, 0.0f,      1.0f,  1.0f, 1.0f,  // Top right
            0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f,      1.0f,  1.0f, 0.0f,  // Bottom right
            -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,      1.0f,  0.0f, 0.0f,  // Bottom left
            -0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 0.0f,      1.0f,  0.0f, 1.0f   // Top left
    };

    unsigned int indices[] = {
            0, 1, 2,  // First triangle
            0, 2, 3   // Second triangle
    };

    // 5: Set up VAO, VBO, EBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Vertex colors
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // 6: Render loop
    while (!glfwWindowShouldClose(window)) {

        // Clear the screen
        glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 7: Draw background
        BGShader.use();
        BGShader.setFloat("uTime", glfwGetTime());
        BGTexture.Bind(0);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // 8: Draw character
        characterShader.use();
        characterShader.setFloat("uTime", glfwGetTime());
        characterTexture.Bind(0);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate program
    glfwTerminate();
}