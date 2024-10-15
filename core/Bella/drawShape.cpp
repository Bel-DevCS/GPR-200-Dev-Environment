//
// Created by Bella on 2024-10-15.
//

#include "drawShape.h"

namespace Bella_GPR200
{
    unsigned int DrawShape::Triangle()
    {
        // Define the triangle vertices (positions, colors, texture coordinates)
        float vertices[] = {
                // Positions      // Colors           // Texture Coords
                0.0f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.5f, 1.0f, // Top vertex
                -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,  0.0f, 0.0f, // Bottom left
                0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f,  1.0f, 0.0f  // Bottom right
        };

        unsigned int VAO, VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        // Bind and set vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Set vertex attribute pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // Position
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // Color
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(7 * sizeof(float))); // Texture coords
        glEnableVertexAttribArray(2);

        glBindVertexArray(0); // Unbind VAO

        return VAO; // Return the VAO
    }
    unsigned int DrawShape::Quad()
    {
        // Define quad vertices (positions, colors, texture coordinates)
        float vertices[] = {
                // Positions        // Colors           // Texture Coords
                0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Top right
                0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 0.0f, // Bottom right
                -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f,  0.0f, 0.0f, // Bottom left
                -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f, 1.0f,  0.0f, 1.0f  // Top left
        };

        // Define the quad indices (for 2 triangles)
        unsigned int indices[] = {
                0, 1, 3, // First triangle
                1, 2, 3  // Second triangle
        };

        unsigned int VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // Bind and set vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Bind and set element buffer (indices)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Set vertex attribute pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0); // Position
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float))); // Color
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float))); // Texture coords
        glEnableVertexAttribArray(2);

        glBindVertexArray(0); // Unbind VAO

        return VAO; // Return the VAO
    }
    unsigned int DrawShape::Cube()
    {
        // Cube vertices (positions, colors, texture coordinates)
        float vertices[] =
                {
                // Positions          // Colors           // Texture Coords
                // Front face
                0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Top right
                0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 0.0f, // Bottom right
                -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f, 1.0f,  0.0f, 0.0f, // Bottom left
                -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f, 1.0f,  0.0f, 1.0f, // Top left

                // Back face
                0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Top right
                0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 0.0f, // Bottom right
                -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f, 1.0f,  0.0f, 0.0f, // Bottom left
                -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f, 1.0f,  0.0f, 1.0f, // Top left

                // Left face
                -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Top right
                -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f, 1.0f,  1.0f, 0.0f, // Bottom right
                -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f, 1.0f,  0.0f, 0.0f, // Bottom left
                -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f, // Top left

                // Right face
                0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Top right
                0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f, 1.0f,  1.0f, 0.0f, // Bottom right
                0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f, 1.0f,  0.0f, 0.0f, // Bottom left
                0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f, // Top left

                // Top face
                0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Top right
                -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f, 1.0f,  1.0f, 0.0f, // Bottom right
                -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f, 1.0f,  0.0f, 0.0f, // Bottom left
                0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f, // Top left

                // Bottom face
                0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Top right
                -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f, 1.0f,  1.0f, 0.0f, // Bottom right
                -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f, 1.0f,  0.0f, 0.0f, // Bottom left
                0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f  // Top left
        };


        unsigned int indices[] = {
                // Front face
                0, 1, 3,
                1, 2, 3,
                // Back face
                4, 5, 7,
                5, 6, 7,
                // Left face
                8, 9, 11,
                9, 10, 11,
                // Right face
                12, 13, 15,
                13, 14, 15,
                // Top face
                16, 17, 19,
                17, 18, 19,
                // Bottom face
                20, 21, 23,
                21, 22, 23
        };

        // 1. Create VAO, VBO, and EBO
        unsigned int VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // 2. Bind VAO
        glBindVertexArray(VAO);

        // 3. Bind and set vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // 4. Bind and set element buffer (indices)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // 5. Set vertex attribute pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0); // Position
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float))); // Color
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float))); // Texture coords
        glEnableVertexAttribArray(2);

        // 6. Unbind the VAO to prevent accidental modifications
        glBindVertexArray(0);

        return VAO; // Return the VAO so it can be used in the render loop
    }
}
