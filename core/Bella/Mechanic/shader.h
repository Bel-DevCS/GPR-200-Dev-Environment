#pragma once
#include "../../ew/external/glad.h"
#include "glm/glm.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace Bella_GPR200
{
    class Shader
    {
        public:
            void setBool(const std::string &name, bool value) const;
            void setInt(const std::string &name, int value) const;
            void setFloat(const std::string &name, float value) const;
            void setMat4(const std::string &name, const glm::mat4 &mat) const;
            void setVec4(const std::string &name, const glm::vec4 &value) const;

            Shader(const char* vertexPath, const char* fragmentPath);

            void use() {glUseProgram(ID);}

            unsigned int ID;
    };
}