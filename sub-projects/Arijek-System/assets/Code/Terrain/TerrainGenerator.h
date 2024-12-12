#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H

#include "Bella/Mechanic/Shader/shader.h"
#include "Bella/Mechanic/Camera/camera.h"
#include "ew/external/glad.h"
#include <vector>

class TerrainGenerator
{
public:
    TerrainGenerator(int gridSize, float scale, int seed);

    void Render(Bella_GPR200::Camera& camera, int width, int height);

private:
    int gridSize;
    float scale;
    int seed;

    float minHeight;
    float maxHeight;

    unsigned int VAO, VBO, EBO;
    Bella_GPR200::Shader NoiseShader;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    float GenerateNoise(int x, int z);
    void InitializeBuffers();

    void SmoothHeightMap(std::vector<std::vector<float>>& heightMap);
};

#endif
