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

namespace Bella_GPR200
{
    class Model
    {
        public:

        explicit Model(const char *path, Shader& shader) : mShader(shader) { loadModel(path); }

        void Draw();

        Shader& getShader() { return mShader; }

    private:
        std::vector<Mesh> meshes;
        std::string directory;

        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);

        std::vector<Texture> loadMaterialTextures
        (aiMaterial *mat, aiTextureType type, std::string typeName);

        std::vector<Texture> textures_loaded;

        Shader mShader;

        void setShader(Shader& shader) { mShader = shader; }

        unsigned int GenerateTexture(const char* path, const std::string& directory);
    };
}
