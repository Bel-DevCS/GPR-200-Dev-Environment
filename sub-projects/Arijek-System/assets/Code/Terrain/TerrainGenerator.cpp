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
}

// Simple noise function
float TerrainGenerator::GenerateNoise(int x, int z) {
    float nx = x * scale;
    float nz = z * scale;
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX); // Replace with better noise as needed
}

// Generate terrain mesh directly (centered)
std::vector<float> TerrainGenerator::GenerateVertices() {
    std::vector<float> vertices;


    for (int x = 0; x < gridSize; ++x) {
        for (int z = 0; z < gridSize; ++z) {
            float height = GenerateNoise(x, z);
            vertices.push_back(x);
            vertices.push_back(height);
            vertices.push_back(z);
        }
    }
    return vertices;
}

std::vector<unsigned int> TerrainGenerator::GenerateIndices()
{
    std::vector<unsigned int> indices;

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

    return indices;
}

void TerrainGenerator::Render(Bella_GPR200::Camera& camera, int width, int height) {

    std::vector<float> vertices = GenerateVertices();
    std::vector<unsigned int> indices = GenerateIndices();

    if (VAO == 0) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // Bind vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        std::vector<float> vertices = GenerateVertices();
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Bind index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        std::vector<unsigned int> indices = GenerateIndices();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    glBindVertexArray(VAO);
    glm::mat4 projection = camera.GetProjectionMatrix((float)width / (float)height);

    // Set up the model, view, and projection matrices
    glm::mat4 model = glm::mat4(1.0f); // Identity matrix
    glm::mat4 view = camera.GetViewMatrix();

    NoiseShader.use();
    NoiseShader.setMat4("model", model);
    NoiseShader.setMat4("view", view);
    NoiseShader.setMat4("projection", projection);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
