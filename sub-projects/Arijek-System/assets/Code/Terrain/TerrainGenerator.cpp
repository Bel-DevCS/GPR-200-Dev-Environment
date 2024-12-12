#include "TerrainGenerator.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <ostream>

// Constructor: Initialize terrain generator
TerrainGenerator::TerrainGenerator(int gridSize, float scale, int seed)
    : gridSize(gridSize), scale(scale), seed(seed), VAO(0), VBO(0),
      minHeight(FLT_MAX), maxHeight(FLT_MIN),
      NoiseShader("assets/Shaders/TerrainShader/TerrainVert.vert",
                  "assets/Shaders/TerrainShader/TerrainFrag.frag")
{
    srand(seed); // Set random seed for consistent noise generation

    // Step 1: Generate initial heightmap using random noise
    std::vector<std::vector<float>> heightMap(gridSize + 1, std::vector<float>(gridSize + 1));
    for (int x = 0; x <= gridSize; ++x) {
        for (int z = 0; z <= gridSize; ++z) {
            heightMap[x][z] = GenerateNoise(x, z);

            // Track the minimum and maximum height values for scaling later
            minHeight = std::min(minHeight, heightMap[x][z]);
            maxHeight = std::max(maxHeight, heightMap[x][z]);
        }
    }

    // Step 2: Smooth and adjust heightmap to create steps
    SmoothHeightMap(heightMap);

    // Step 3: Convert heightmap into vertex data
    for (int x = 0; x <= gridSize; ++x) {
        for (int z = 0; z <= gridSize; ++z) {
            float height = heightMap[x][z];
            vertices.push_back((x - gridSize / 2.0f) * scale); // X position
            vertices.push_back(height);                        // Y position (height)
            vertices.push_back((z - gridSize / 2.0f) * scale); // Z position
        }
    }

    // Step 4: Generate indices for terrain triangles
    for (int x = 0; x < gridSize; ++x) {
        for (int z = 0; z < gridSize; ++z) {
            unsigned int topLeft = x + z * (gridSize + 1);
            unsigned int topRight = topLeft + 1;
            unsigned int bottomLeft = topLeft + (gridSize + 1);
            unsigned int bottomRight = bottomLeft + 1;

            // Create two triangles for each quad
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    // Initialize OpenGL buffers with the generated data
    InitializeBuffers();
}

// Smooth the heightmap to create more natural terrain
void TerrainGenerator::SmoothHeightMap(std::vector<std::vector<float>>& heightMap) {
    std::vector<std::vector<float>> tempMap = heightMap;

    // Smooth each non-border cell by averaging its neighbors
    for (int x = 1; x < gridSize; ++x) {
        for (int z = 1; z < gridSize; ++z) {
            float sum = heightMap[x][z] +
                        heightMap[x - 1][z] +
                        heightMap[x + 1][z] +
                        heightMap[x][z - 1] +
                        heightMap[x][z + 1];
            tempMap[x][z] = sum / 5.0f; // Average value
        }
    }

    // Apply stepping for blocky aesthetics (e.g., PS1-style)
    float stepSize = 1.0f;
    for (int x = 0; x <= gridSize; ++x) {
        for (int z = 0; z <= gridSize; ++z) {
            tempMap[x][z] = round(tempMap[x][z] / stepSize) * stepSize;
        }
    }

    heightMap = tempMap; // Update the original heightmap
}

// Generate noise for heightmap
float TerrainGenerator::GenerateNoise(int x, int z)
{
    float rawNoise = static_cast<float>(rand()) / static_cast<float>(RAND_MAX); // Random noise
    float stepSize = 0.1f;
    return round(rawNoise / stepSize) * stepSize * 3.0f; // Scale and step noise values
}

// Initialize OpenGL buffers for rendering the terrain
void TerrainGenerator::InitializeBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Load vertex data into GPU
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Load index data into GPU
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Render the terrain with the specified camera and viewport
void TerrainGenerator::Render(Bella_GPR200::Camera& camera, int width, int height) {
    glm::mat4 projection = camera.GetProjectionMatrix((float)width / (float)height);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f); // Identity matrix

    // Configure dynamic properties like fog and lighting
    float snowHeight = maxHeight * 0.4f;
    float iceHeight = minHeight + (maxHeight - minHeight) * 0.2f;

    //Setting Camera Uniforms
    NoiseShader.use();
    NoiseShader.setMat4("model", model);
    NoiseShader.setMat4("view", view);
    NoiseShader.setMat4("projection", projection);

    //Setting Light Uniform
    NoiseShader.setVec3("lightDir", glm::normalize(glm::vec3(-0.3f, -1.0f, -0.3f)));
    NoiseShader.setVec3("lightColor", glm::vec3(0.9f, 0.9f, 1.0f));

    //Setting Fog Uniforms
    NoiseShader.setVec3("fogColor", glm::vec3(0.7f, 0.8f, 0.9f));   // Fog color
    NoiseShader.setFloat("fogStart", 3.0f);
    NoiseShader.setFloat("fogEnd", 10.0f);

    //Setting Uniforms for Gradient / Colour Mapping
    NoiseShader.setFloat("iceHeight", iceHeight);
    NoiseShader.setFloat("snowHeight", snowHeight);

    //Draw Call
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}