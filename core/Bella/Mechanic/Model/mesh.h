#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "../shader.h"

namespace Bella_GPR200
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    struct Texture
    {
        unsigned int id;
        std::string type;
        std::string path;
    };

    class Mesh
    {
        public:
        std::vector<Vertex> verts;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void Draw(Shader &shader);

        void setMaterialColor(const glm::vec3& color);

        private:

         unsigned int VAO, VBO, EBO;

        glm::vec3 materialColor;
        bool useMaterialColor = false;

         void setupMesh();

    };
}