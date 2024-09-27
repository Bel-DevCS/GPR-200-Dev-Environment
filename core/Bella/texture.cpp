//
// Created by Bella Wunsch on 2024-09-26.
//

#include "texture.h"
namespace Bella_GPR200
{
    Texture2D::Texture2D(const char *filePath, int filterMode, int wrapMode)
    {
        //1 : Generate Texture Object
        glGenTextures(1, &m_id);

        //2 : Bind Texture
        glBindTexture(GL_TEXTURE_2D, m_id);

        //3 : Load Image Data
        unsigned char* data = stbi_load(filePath, &m_width, &m_height, &m_nrChannels, 0);


        //4 : Check if Data Failed to Generate
        if (data) {
            GLenum format = (m_nrChannels == 4) ? GL_RGBA : GL_RGB;

            // 4(a) : Format Texture
            glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            //4(b) : Wrap Texture
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

            //4(c) : Filter Texture
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
        }

        else {  std::cerr << "Failed to load texture: " << filePath << std::endl; }


        //5 : Clear Data
        stbi_image_free(data);
    }

    Texture2D::~Texture2D() { glDeleteTextures(1, &m_id);}

    void Texture2D::Bind(unsigned int slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }
}