//
// Created by Kearura on 2024-10-24.
//

#pragma once

#include <iostream>
#include <vector>
#include "../Shader/shader.h"
#include "mesh.h"
#include "../Texture/texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../../../ew/external/stb_image.h"

// Model class Written by Bella
namespace Bella_GPR200
{
    class Model
    {
        public:

        explicit Model(const char *path, Shader& shader) : mShader(shader) { loadModel(path); }
        // Constructor by Anne
        explicit Model(std::string name, const char *path, Shader& shader) : mName(name), mShader(shader) { loadModel(path); }

        void Draw();

        // Get and Set methods by Anne
        Shader& getShader() { return mShader; }
        void setShader(Shader& shader) { mShader = shader; }

        void setPosition(glm::vec3 position) { mPosition = position; }
        glm::vec3 getPosition() const { return mPosition; }

        void setScale(glm::vec3 scale) { mScale = scale; }
        glm::vec3 getScale() const { return mScale; }

        void setRotation(glm::vec3 rotation) { mRotation = rotation; }
        glm::vec3 getRotation() const { return mRotation; }

        void setVisibility(bool visibility) { mVisibility  = visibility; }
        bool getVisibility() const { return mVisibility ; }

        std::string getName() const { return mName; }

    private:
        std::vector<Mesh> meshes;
        std::string directory;

        // Pos, Scale, Rot by Anne
        glm::vec3 mPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 mScale = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 mRotation = glm::vec3(0.0f, 0.0f,0.0f);

        // Visibility by anne
        bool mVisibility = true;

        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);

        std::vector<Texture> loadMaterialTextures
        (aiMaterial *mat, aiTextureType type, std::string typeName);

        std::vector<Texture> textures_loaded;

        // Model containing shader refactor by Anne
        Shader mShader;

        // Name by Anne
        const std::string mName;

        unsigned int GenerateTexture(const char* path, const std::string& directory);
    };
}
