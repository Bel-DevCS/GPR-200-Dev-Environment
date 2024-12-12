#include "TerrainGenerator.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <ostream>

// Constructor
TerrainGenerator::TerrainGenerator(int gridSize, float scale, int seed)
    : gridSize(gridSize), scale(scale), seed(seed), VAO(0), VBO(0), NoiseShader
        (
        "assets/Shaders/TerrainShader/TerrainVert.vert",
        "assets/Shaders/TerrainShader/TerrainFrag.frag"
        )
{
    srand(seed);

    // Generate vertices and indices once
    for (int x = 0; x < gridSize; ++x) {
        for (int z = 0; z < gridSize; ++z) {
            float height = GenerateNoise(x, z);
            vertices.push_back(x - gridSize / 2.0f); // Center terrain
            vertices.push_back(height);
            vertices.push_back(z - gridSize / 2.0f); // Center terrain
        }
    }

    for (int x = 0; x < gridSize - 1; ++x) {
        for (int z = 0; z < gridSize - 1; ++z) {
            unsigned int i = x + z * gridSize;
            indices.push_back(i);
            indices.push_back(i + 1);
            indices.push_back(i + gridSize);

            indices.push_back(i + 1);
            indices.push_back(i + gridSize + 1);
            indices.push_back(i + gridSize);
        }
    }

    InitializeBuffers();
}

// Simple noise function
float TerrainGenerator::GenerateNoise(int x, int z)
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

void TerrainGenerator::InitializeBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Bind vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Bind index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TerrainGenerator::Render(Bella_GPR200::Camera& camera, int width, int height) {
    glm::mat4 projection = camera.GetProjectionMatrix((float)width / (float)height);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);

    NoiseShader.use();
    NoiseShader.setMat4("model", model);
    NoiseShader.setMat4("view", view);
    NoiseShader.setMat4("projection", projection);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
