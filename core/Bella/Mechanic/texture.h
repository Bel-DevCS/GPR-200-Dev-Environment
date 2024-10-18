//
// Created by Bella Wunsch on 2024-09-26.
//

#pragma once
#include "../../ew/external/stb_image.h"
#include <string>
#include "../../ew/external/glad.h"
#include <iostream>

namespace Bella_GPR200
{
    class Texture2D {
    public:
        Texture2D(const char *filePath, int filterMode = GL_LINEAR, int wrapMode = GL_REPEAT);

        ~Texture2D();

        void Bind(unsigned int slot = 0) const;

    private:
        unsigned int m_id;
        int m_width, m_height, m_nrChannels;
    };
}
