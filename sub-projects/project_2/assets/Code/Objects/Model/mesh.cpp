#include "mesh.h"

namespace Bella_Engine {

    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
        : verts(vertices), indices(indices), textures(textures), VAO(0), VBO(0), EBO(0) {
        setupMesh();
    }

    void Mesh::Initialize() {
        // Check if VAO is already generated to avoid redundant initialization
        if (VAO == 0) {
            std::cout << "Initializing mesh..." << std::endl;
            setupMesh();
        } else {
            std::cout << "Mesh already initialized." << std::endl;
        }
    }

    void Mesh::Update(float deltaTime) {
        // Example: Could implement mesh-specific animations or transformations
        // For now, it's a placeholder to fit with the scene's update cycle
        // In a real case, you could add transformations, skinning, etc.
    }

    void Mesh::Draw(Shader& shader) {
        // Bind appropriate textures if they exist
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;

        for (unsigned int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);  // Activate texture unit

            std::string number;
            std::string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);

            // Set the sampler to the corresponding texture unit
            shader.setInt((name + number).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        // Set material color if specified or if no textures are available
        if (useMaterialColor || textures.empty()) {
            shader.setVec3("materialColor", materialColor);
            shader.setInt("useMaterialColor", 1);
        } else {
            shader.setInt("useMaterialColor", 0);
        }

        // Draw the mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Set back to default
        glActiveTexture(GL_TEXTURE0);
    }

    void Mesh::Cleanup() {
        // Delete the buffers and VAO to free up resources
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);

        std::cout << "Mesh cleanup complete." << std::endl;
    }

    void Mesh::setMaterialColor(const glm::vec3& color) {
        materialColor = color;
        useMaterialColor = true;
    }

    void Mesh::setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), &verts[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // Vertex attributes
        // Vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // Vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

        // Vertex texture coordinates
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);
    }
}
