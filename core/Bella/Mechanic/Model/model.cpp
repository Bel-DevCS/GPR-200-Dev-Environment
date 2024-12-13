//
// Created by Kearura on 2024-10-24.
//

#include "model.h"

#include "../../../../cmake-build-debug/_deps/assimp-build/include/assimp/config.h"
#include "glm/ext/matrix_transform.hpp"

namespace Bella_GPR200
{
    void Model::Draw()
    {
        // send model to shader
        glm::mat4 model = glm::mat4 (1.0f);
        model = glm::translate(model, mPosition);
        model = glm::scale(model, mScale);

        model = glm::rotate(model, glm::radians(mRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(mRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(mRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));

        mShader.setMat4("model", model);

        for (unsigned int i = 0; i < meshes.size(); i++)
        {
            // Check if the mesh has textures
            if (meshes[i].textures.empty())
            {
                // If no textures, use material color
                mShader.setBool("useTexture", false);
                // Set a default material color for now
                mShader.setVec3("materialColor", glm::vec3(0.5f, 0.5f, 0.5f)); // Example color
            }
            else
            {
                // If textures are present, use them
                mShader.setBool("useTexture", true);
                for (unsigned int j = 0; j < meshes[i].textures.size(); j++)
                {
                    std::string number;
                    std::string name = meshes[i].textures[j].type;
                    if (name == "texture_diffuse")
                        number = "1";  // Assuming one diffuse texture for now
                    mShader.setInt((name + number).c_str(), j);
                    glActiveTexture(GL_TEXTURE0 + j); // Activate texture unit
                    glBindTexture(GL_TEXTURE_2D, meshes[i].textures[j].id);
                }
            }
            meshes[i].Draw(mShader);
        }
    }


    void Model::loadModel(std::string path)
    {
        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    }

    void Model::processNode(aiNode *node, const aiScene *scene)
    {
        // process all the node's meshes (if any)
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // then do the same for each of its children
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

   Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    glm::vec3 materialColor = glm::vec3(0.5f, 0.5f, 0.5f); // Default color

    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;

        // Positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        // Normals
        if (mesh->mNormals)
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        // Texture Coordinates
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // Process materials
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // Load diffuse textures
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        // Load specular textures
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        // If no diffuse textures, try to load the material color
        if (diffuseMaps.empty())
        {
            aiColor3D color(0.5f, 0.5f, 0.5f); // Default color
            if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, color))
            {
                materialColor = glm::vec3(color.r, color.g, color.b);
            }
        }
    }

    // Create mesh with vertices, indices, and textures
    Mesh meshResult(vertices, indices, textures);

    // Set the material color for the mesh if no textures
    if (textures.empty())
    {
        meshResult.setMaterialColor(materialColor);
    }

    return meshResult;
}


    std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
    {

        std::vector<Texture> textures;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;
            for(unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    break;
                }
            }
            if(!skip)
            {   // if texture hasn't been loaded already, load it
                Texture texture;
                texture.id = GenerateTexture(str.C_Str(), directory);
                texture.type = typeName;
                texture.path = std::string(str.C_Str());
                textures.push_back(texture);
                textures_loaded.push_back(texture); // add to loaded textures
            }
        }

        return textures;
    }




    //Replaces TextureFromFile
    unsigned int Model::GenerateTexture(const char* path, const std::string& directory)
    {
        std::string filename = directory + '/' + std::string(path);

        // Create a Texture2D object and load the texture
        Bella_GPR200::Texture2D texture(filename.c_str());

        // Return the OpenGL texture ID
        return texture.getID();
    }

}
