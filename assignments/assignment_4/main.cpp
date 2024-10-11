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

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>


const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

void printMatrix(const glm::mat4& matrix) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            std::cout << matrix[col][row] << " ";
        }
        std::cout << std::endl;
    }
}


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

    //3 : Create Shape
    float vertices[] =
            {
                    //X     Y     Z         R     G     B      A
                    -0.5f, -0.5f, 0.0f,    1.0f, 0.302f, 0.506f, 1.0f, //Magenta
                    0.5f, -0.5f, 0.0f,     0.98, 1.0f, 0.302f, 1.0f, //Yellow
                    0.0f,  0.5f, 0.0f,     0.537f, 0.8f, 1.0f, 1.0f //Cyan
            };



    //Matrix Practice
    glm::mat4 demoMatrix = glm::mat4(1.0f); //Identity Matrix
     std::cout << "Initial Matrix (Identity): " << std::endl;
     std::cout << glm::to_string(demoMatrix) << std::endl;

    demoMatrix = glm::scale(demoMatrix, glm::vec3(2.0f, 3.0f, 1.0f));

    std::cout << "After Scaling (2x, 3y): " << std::endl;
    std::cout << glm::to_string(demoMatrix) << std::endl;

    float angleInRadians = glm::radians(45.0f); // Convert degrees to radians
    demoMatrix = glm::rotate(demoMatrix, angleInRadians, glm::vec3(0.0f, 0.0f, 1.0f));

    std::cout << "After Rotation (45 degrees Z-axis): " << std::endl;
    std::cout << glm::to_string(demoMatrix) << std::endl;

    demoMatrix = glm::translate(demoMatrix, glm::vec3(1.0f, 2.0f, 0.0f));

    std::cout << "After Translation (+1x, +2y): " << std::endl;
    std::cout << glm::to_string(demoMatrix) << std::endl;


    for(int i = 0; i < 3; i++)
        std::cout << std::endl;


    glm::mat4 manualMatrix = glm::mat4(1.0f);
    printMatrix(manualMatrix);
    std::cout << "\n\n\n";

    manualMatrix[0][0] = 2.0f;  // X-axis scaling
    manualMatrix[1][1] = 3.0f;  // Y-axis scaling
    manualMatrix[2][2] = 1.0f;  // Z-axis scaling
    printMatrix(manualMatrix);
    std::cout << "\n\n\n";

    float angle = glm::radians(45.0f);  // Convert to radians
    manualMatrix[0][0] = cos(angle);   // cos(θ)
    manualMatrix[1][0] = sin(angle);   // sin(θ)
    manualMatrix[0][1] = -sin(angle);  // -sin(θ)
    manualMatrix[1][1] = cos(angle);
    printMatrix(manualMatrix);
    std::cout << "\n\n\n";

    //4 : Instantiate Vertex Array Object, and Vertex Buffer Object
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //5 : Bind Vertex Array Object
    glBindVertexArray(VAO);

    //6 : Bind Vertex Array Object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //7 : Attribute Pointer for Position (X,Y,Z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //8 : Attribute Pointer for Colour (R,G,B,A)
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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
        glBindVertexArray(VAO);

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        ourShader.setMat4("uTransform", transform);


        //9(d) : Draw Call
        glDrawArrays(GL_TRIANGLES, 0, 3);


        //9(e) : Swap Buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    //10 : Terminate Program
    printf("Shutting down...");
    glfwTerminate();

}
