#include "mesh.h"


namespace Bella_GPR200
{
    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
    {
        this->verts = vertices;
        this->indices = indices;
        this->textures = textures;

        setupMesh();
    }

    void Mesh::setMaterialColor(const glm::vec3& color)
    {
        materialColor = color;
        useMaterialColor = true; // Set the flag to use material color
    }

    void Mesh::setupMesh()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), &verts[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                     &indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);
    }

    void Mesh::Draw(Shader& shader)
    {
        // Bind appropriate textures if they exist
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;

        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding

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

        // Set the material color if no textures are available or if explicitly set
        if (useMaterialColor || textures.empty())
        {
            shader.setVec3("materialColor", materialColor);
            shader.setInt("useMaterialColor", 1);
        }
        else
        {
            shader.setInt("useMaterialColor", 0);
        }

        // Draw the mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Set back to default
        glActiveTexture(GL_TEXTURE0);
    }


}
