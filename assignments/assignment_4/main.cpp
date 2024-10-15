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

    Bella_GPR200::Texture2D aText("assets/Images/Example_Images/amythest.png");

    //3 : Create Shape
    float vertices[] =
            {
            //       X     Y     Z       R     G     B      A        S    T     R
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


    float texCoords[] =
            {
                    0.0f, 0.0f,
                    1.0f, 0.0f,
                    0.0f, 1.0f,
                    1.0f, 1.0f
            };


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

    //Matrices?
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
// note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);


    //9 : Render Loop
    while (!glfwWindowShouldClose(window)) {

        //9(a) : Clear the Screen
        glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //9(b) : Instantiate Shader Uniforms
        float timeValue = glfwGetTime();
        ourShader.setFloat("uTime", timeValue);


        //9(c) : Use Shader and Bind Vertex Array to Shader
        aText.Bind(0);
        ourShader.use();
        glBindVertexArray(VAO);


        //glm::mat4 transform = glm::mat4(1.0f);
        //transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        //ourShader.setMat4("uTransform", model);

        ourShader.setMat4("model", model);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);


        //9(d) : Draw Call
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        //9(e) : Swap Buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    //10 : Terminate Program
    printf("Shutting down...");
    glfwTerminate();

}
