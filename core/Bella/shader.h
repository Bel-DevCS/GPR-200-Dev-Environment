#pragma once
#include "../ew/external/glad.h"
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

            Shader(const char* vertexPath, const char* fragmentPath);

            void use() {glUseProgram(ID);}

            unsigned int ID;
    };
}