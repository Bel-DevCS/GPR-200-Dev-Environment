#pragma once
#include "../../../ew/external/glad.h"
#include "glm/glm.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

#include "../Lighting/Light Class/light.h"

namespace Bella_GPR200
{
    namespace Lighting
    {
        class Light;
    }

    class Shader
    {
        public:
            unsigned int ID;
            Shader(const char* vertexPath, const char* fragmentPath);

            // Feature toggles
            void AddLighting(Bella_GPR200::Lighting::Light light);
          //  void AddTexture(const std::string& texturePath);

            //Set Uniforms
            void setBool(const std::string &name, bool value) const;
            void setInt(const std::string &name, int value) const;
            void setFloat(const std::string &name, float value) const;
            void setMat4(const std::string &name, const glm::mat4 &mat) const;
            void setVec3(const std::string &name, const glm::vec3 &value) const;
            void setVec4(const std::string &name, const glm::vec4 &value) const;

            void use() {glUseProgram(ID);}

        private:
        bool hasLighting = false;
        bool hasTexture = false;
        std::string lightingCode;
        std::string textureCode;

        std::string fragmentCode;
        std::string vertexCode;

        void ReplacePlaceholder(std::string& shaderCode, const std::string& placeholder, const std::string& replacement);
        void CompileShader(const std::string& vertexCode, const std::string& fragmentCode);
        std::string LoadShaderCode(const char* filePath);

    };
}