#pragma once

#include "Bella/Mechanic/Shader/shader.h"
#include "Bella/Mechanic/Camera/camera.h"
#include "ew/external/glad.h"
#include <vector>

class TerrainGenerator
{
public:
    // Constructor: Initializes terrain with a grid size, scale, and random seed
    TerrainGenerator(int gridSize, float scale, int seed);

    // Render the terrain using the provided camera and viewport dimensions
    void Render(Bella_GPR200::Camera& camera, int width, int height);

    // Accessors for grid size and scale (useful for external configurations)
    int GetGridSize() const { return gridSize; }
    float GetScale() const { return scale; }

private:
    int gridSize; // Number of grid cells in the terrain
    float scale; // Scale factor for spacing vertices
    int seed; // Random seed for consistent terrain generation -> for project use 7


    float minHeight; // Lowest height value in the terrain
    float maxHeight;  // Highest height value in the terrain

    unsigned int VAO, VBO, EBO; // OpenGL buffers for rendering
    Bella_GPR200::Shader NoiseShader;  // Shader used for terrain rendering

    std::vector<float> vertices; // Vertex data for terrain mesh
    std::vector<unsigned int> indices; // Index data for terrain mesh

    // Generate a random noise value for terrain height at (x, z)
    float GenerateNoise(int x, int z);

    // Smooth the heightmap to create a more natural terrain
    void SmoothHeightMap(std::vector<std::vector<float>>& heightMap);

    // Initialize OpenGL buffers for the terrain mesh
    void InitializeBuffers();
};
