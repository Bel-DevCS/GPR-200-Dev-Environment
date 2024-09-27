#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

#include <Bella/shader.h>

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

    //2 : Instantiate Shader
    Bella_GPR200::Shader ourShader("assets/vertexShader.vert", "assets/fragmentShader.frag");

    //3 : Create Shape
    float vertices[] =
            {
            //       X     Y     Z         R     G     B     A       S    T
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

    //Texture
    float texCoords[] =
            {
                    0.0f, 0.0f,
                    1.0f, 0.0f,
                    0.0f, 1.0f,
                    1.0f, 1.0f
            };

    //More Texture Stuff, I don't know where it goes yet
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); //Specify Texture Target
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); //Specifies how texture is applied

    //glTexParameteri (Texture Target, Which Option for which axis [S,T,R == X,Y,Z], Texture Wrap Type)
    //TexParameteri -> Texture Parameter Integer

    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    //glTexParameterfv(Texture Target, Action, Applied Filter / Action)

    //Todo : replace implementation with class
    unsigned int texture;
    glGenTextures(1, &texture);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("assets/Images/ExampleImages/container.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture);  // Bind the texture

        // Upload the texture data to the GPU
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);  // Generate mipmaps

        // Set texture parameters (wrapping, filtering)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        std::cout << "Failed to load texture!" << std::endl;
    }
    stbi_image_free(data);  // Free the image data after uploading



    //4 : Instantiate Vertex Array Object, and Vertex Buffer Object
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //5 : Initiate Element Buffer Object
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    //6 : Bind Vertex Array Object
    glBindVertexArray(VAO);

    //7 : Bind Vertex Array Object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //8 : Bind Element Buffer Object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //9 : Attribute Pointer for Position (X,Y,Z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //10 : Attribute Pointer for Colour (R,G,B,A)
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Attribute Pointer for Texture (S,T)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //11 : Render Loop
    while (!glfwWindowShouldClose(window)) {

        //11(a) : Clear the Screen
        glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //11(b) : Instantiate Shader Uniforms
        float timeValue = glfwGetTime();
        ourShader.setFloat("uTime", timeValue);


        //11(c) : Use Shader and Bind Vertex Array to Shader
        ourShader.use();
        glBindVertexArray(VAO);

        //11(d) : Draw Call
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        //11(e) : Swap Buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    //12 : Terminate Program
    printf("Shutting down...");
    glfwTerminate();

}
