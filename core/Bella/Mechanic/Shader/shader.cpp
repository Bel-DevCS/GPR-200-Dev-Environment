#include "shader.h"
namespace Bella_GPR200 {
    Shader::Shader(const char *vertexPath, const char *fragmentPath)
    {
        {
            // Step 1: Load shader code from files
             vertexCode = LoadShaderCode(vertexPath);
             fragmentCode = LoadShaderCode(fragmentPath);

            // Step 2 : Compile Shader
            CompileShader(vertexCode, fragmentCode);
        }
    }


    std::string Shader::LoadShaderCode(const char* filePath) {
        std::ifstream shaderFile;
        std::stringstream shaderStream;

        // Set exception mask for the file stream
        shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            // Open the file
            shaderFile.open(filePath);
            // Read the file's content into the stream
            shaderStream << shaderFile.rdbuf();
            // Close the file after reading
            shaderFile.close();
        } catch (std::ifstream::failure& e) {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << filePath << std::endl;
        }

        std::cout << "Successfully read file\n";

        // Convert the stream buffer to a string and return it
        return shaderStream.str();
    }


    void Shader::CompileShader(const std::string& vertexCode, const std::string& fragmentCode) {
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        // Vertex Shader Compilation
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);

        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // Fragment Shader Compilation
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // Shader Program Linking
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        // Delete shaders as they’re linked into the program now and no longer needed
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }



    void Shader::setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
    }

    void Shader::setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void Shader::setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
    }

    void Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z, value.w);
    }

}