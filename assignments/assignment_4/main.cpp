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

    unsigned int cubeVAO = Bella_GPR200::DrawShape::Cube();

    //Matrices?
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.5f, 1.0f, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
// note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);



    //9 : Render Loop
    while (!glfwWindowShouldClose(window)) {

        glEnable(GL_DEPTH_TEST);

        //9(a) : Clear the Screen
        glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //9(b) : Instantiate Shader Uniforms
        float timeValue = glfwGetTime();
        ourShader.setFloat("uTime", timeValue);


        //9(c) : Use Shader and Bind Vertex Array to Shader
        aText.Bind(0);
        ourShader.use();
        glBindVertexArray(cubeVAO);


        //glm::mat4 transform = glm::mat4(1.0f);
        //transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        //ourShader.setMat4("uTransform", model);

        model = glm::rotate(model, timeValue / 5 * glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));


        ourShader.setMat4("model", model);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);


        //9(d) : Draw Call
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        //9(e) : Swap Buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    //10 : Terminate Program
    printf("Shutting down...");
    glfwTerminate();

}
