//
// Created by Kearura on 2024-12-11.
//

#pragma once
#include "Bella/Mechanic/Shader/shader.h"

#include <vector>

#include "Bella/Mechanic/Camera/camera.h"
#include "ew/external/glad.h"
class TerrainGenerator
{
public:
    // Constructor
    TerrainGenerator(int gridSize, float scale, int seed);

    // Generate terrain mesh directly
    std::vector<float> GenerateMesh();

    // Render the terrain
    void Render(Bella_GPR200::Camera& camera, int width, int height);

private:
    int gridSize;       // Size of the grid
    float scale;        // Noise scale
    int seed;           // Seed for consistent results

    unsigned int VAO, VBO; // OpenGL buffers

    Bella_GPR200::Shader NoiseShader;

    // Noise generation
    float GenerateNoise(int x, int z);
};

