#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <glm/glm.hpp>  // Include glm for vector and matrix operations
#include "../shader.h"
#include "mesh.h"
#include "../texture.h"
#include "assimp/scene.h"

namespace Bella_Engine {

    class Model {
    public:
        explicit Model(const char* path, std::shared_ptr<Shader> shader);

        // Core functions
        void Initialize();
        void Update(float deltaTime);
        std::shared_ptr<Shader> GetShader() const;
        void Render(Shader& shader);
        void Draw(Shader& shader);
        void Cleanup();

        // Transformation functions
        void SetPosition(const glm::vec3& pos) { position = pos; }
        void SetScale(const glm::vec3& scl) { scale = scl; }
        void SetRotation(const glm::vec3& rot) { rotation = rot; }

        glm::vec3 GetPosition() const { return position; }
        glm::vec3 GetScale() const { return scale; }
        glm::vec3 GetRotation() const { return rotation; }
        const std::vector<Mesh>& GetMeshes() const { return meshes; }

    private:
        std::vector<Mesh> meshes;
        std::string directory;
        std::shared_ptr<Shader> shader;

        glm::vec3 position;  // Position of the model
        glm::vec3 scale;     // Scale of the model
        glm::vec3 rotation;  // Rotation of the model

        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);

        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
        std::vector<Texture> textures_loaded;

        unsigned int GenerateTexture(const char* path, const std::string& directory);
    };
}
