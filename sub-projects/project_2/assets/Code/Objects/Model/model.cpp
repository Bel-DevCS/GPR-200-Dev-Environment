#include "model.h"

#include <glm/ext/matrix_transform.hpp>

#include "../../../../cmake-build-debug/_deps/assimp-build/include/assimp/config.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

namespace Bella_Engine {


    Model::Model(const char* path, std::shared_ptr<Shader> shader)
        : shader(shader), position(0.0f), scale(1.0f), rotation(0.0f)
    {
        loadModel(path);
    }


    void Model::Initialize()
    {
        for (auto& mesh : meshes) {
            mesh.Initialize();
        }
    }

    void Model::Update(float deltaTime)
    {
        for (auto& mesh : meshes) {
            mesh.Update(deltaTime);
        }
    }


    std::shared_ptr<Shader>Model::GetShader() const {
        return shader;
    }

    void Model::Render(Shader& shader) {
        // Create a model matrix
        glm::mat4 modelMatrix = glm::mat4(1.0f);

        // Apply transformations
        modelMatrix = glm::translate(modelMatrix, position);       // Position
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));  // Rotation X
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));  // Rotation Y
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));  // Rotation Z
        modelMatrix = glm::scale(modelMatrix, scale);              // Scale

        // Pass the model matrix to the shader
        shader.setMat4("model", modelMatrix);

        // Draw the model
        Draw(shader);
    }

    void Model::Draw(Shader& shader) {
        for (unsigned int i = 0; i < meshes.size(); i++) {
            if (meshes[i].textures.empty()) {
                shader.setBool("useTexture", false);
                shader.setVec3("materialColor", glm::vec3(0.5f, 0.5f, 0.5f));  // Default color
            } else {
                shader.setBool("useTexture", true);
                for (unsigned int j = 0; j < meshes[i].textures.size(); j++) {
                    std::string number;
                    std::string name = meshes[i].textures[j].type;
                    if (name == "texture_diffuse") number = "1";  // Example number for diffuse texture

                    shader.setInt((name + number).c_str(), j);
                    glActiveTexture(GL_TEXTURE0 + j);
                    glBindTexture(GL_TEXTURE_2D, meshes[i].textures[j].id);
                }
            }
            meshes[i].Draw(shader);
        }
    }

    void Model::Cleanup()
    {
        // Reset the shader shared_ptr
        shader.reset();

        // Cleanup each mesh
        for (auto& mesh : meshes) {
            mesh.Cleanup();
        }

        std::cout << "Model cleanup complete." << std::endl;
    }

    void Model::loadModel(std::string path) {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));
        processNode(scene->mRootNode, scene);
    }

    void Model::processNode(aiNode* node, const aiScene* scene) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        glm::vec3 materialColor = glm::vec3(0.5f, 0.5f, 0.5f);

        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            glm::vec3 vector;

            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            if (mesh->mNormals) {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }

            if (mesh->mTextureCoords[0]) {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            } else {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }

            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        if (mesh->mMaterialIndex >= 0) {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

            if (diffuseMaps.empty()) {
                aiColor3D color(0.5f, 0.5f, 0.5f);
                if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, color)) {
                    materialColor = glm::vec3(color.r, color.g, color.b);
                }
            }
        }

        Mesh meshResult(vertices, indices, textures);
        if (textures.empty()) {
            meshResult.setMaterialColor(materialColor);
        }

        return meshResult;
    }

    std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
        std::vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++) {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    break;
                }
            }
            if (!skip) {
                Texture texture;
                texture.id = GenerateTexture(str.C_Str(), directory);
                texture.type = typeName;
                texture.path = std::string(str.C_Str());
                textures.push_back(texture);
                textures_loaded.push_back(texture);
            }
        }
        return textures;
    }

    unsigned int Model::GenerateTexture(const char* path, const std::string& directory) {
        std::string filename = directory + '/' + std::string(path);
        Bella_Engine::Texture2D texture(filename.c_str());
        return texture.getID();
    }
}
