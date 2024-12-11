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

// Generate terrain mesh directly
std::vector<float> TerrainGenerator::GenerateMesh() {
    std::vector<float> vertices;

    for (int x = 0; x < gridSize - 1; ++x) {
        for (int z = 0; z < gridSize - 1; ++z) {
            float h1 = GenerateNoise(x, z);
            float h2 = GenerateNoise(x + 1, z);
            float h3 = GenerateNoise(x, z + 1);
            float h4 = GenerateNoise(x + 1, z + 1);

            // First triangle
            vertices.push_back(x);
            vertices.push_back(h1);
            vertices.push_back(z);

            vertices.push_back(x + 1);
            vertices.push_back(h2);
            vertices.push_back(z);

            vertices.push_back(x);
            vertices.push_back(h3);
            vertices.push_back(z + 1);

            // Second triangle
            vertices.push_back(x + 1);
            vertices.push_back(h2);
            vertices.push_back(z);

            vertices.push_back(x + 1);
            vertices.push_back(h4);
            vertices.push_back(z + 1);

            vertices.push_back(x);
            vertices.push_back(h3);
            vertices.push_back(z + 1);
        }
    }

    return vertices;
}

// Render terrain
void TerrainGenerator::Render() {
    if (VAO == 0) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        std::vector<float> mesh = GenerateMesh();
        glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(float), mesh.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        std::cout << "num vertices : " << mesh.size() << std::endl;
    }

    glBindVertexArray(VAO);
    NoiseShader.use();
    glDrawArrays(GL_TRIANGLES, 0, gridSize * gridSize * 6);
    glBindVertexArray(0);
}
