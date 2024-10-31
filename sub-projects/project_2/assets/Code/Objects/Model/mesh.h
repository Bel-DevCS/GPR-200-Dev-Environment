#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "../shader.h"

namespace Bella_Engine {

    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    struct Texture {
        unsigned int id;
        std::string type;
        std::string path;
    };

    class Mesh {
    public:
        std::vector<Vertex> verts;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

        // Core functions
        void Initialize();  // Initialize the mesh
        void Update(float deltaTime);  // Update the mesh
        void Draw(Shader& shader);  // Draw the mesh
        void Cleanup();  // Cleanup mesh resources

        void setMaterialColor(const glm::vec3& color);
        bool HasMaterialColor() const { return useMaterialColor; }
        glm::vec3 GetMaterialColor() const { return materialColor; }

    private:
        unsigned int VAO, VBO, EBO;
        glm::vec3 materialColor;
        bool useMaterialColor = false;

        void setupMesh();
    };
}
