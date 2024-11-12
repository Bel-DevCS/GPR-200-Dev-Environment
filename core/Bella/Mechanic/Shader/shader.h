#pragma once
#include "../../../ew/external/glad.h"
#include "glm/glm.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

#include "../Lighting/Light Class/light.h"

namespace Bella_GPR200
{

    class Shader
    {
        public:
            unsigned int ID;
            Shader(const char* vertexPath, const char* fragmentPath);

            //Set Uniforms
            void setBool(const std::string &name, bool value) const;
            void setInt(const std::string &name, int value) const;
            void setFloat(const std::string &name, float value) const;
            void setMat4(const std::string &name, const glm::mat4 &mat) const;
            void setVec3(const std::string &name, const glm::vec3 &value) const;
            void setVec4(const std::string &name, const glm::vec4 &value) const;

            void use() {glUseProgram(ID);}

        private:

        void CompileShader(const std::string& vertexCode, const std::string& fragmentCode);
        std::string LoadShaderCode(const char* filePath);
        std::string vertexCode;
        std::string fragmentCode;

    };
}