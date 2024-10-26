//
// Created by Kearura on 2024-10-26.
//

#pragma once

#include <glm/glm.hpp>
#include "shader.h"

namespace Bella_GPR200
{
    namespace Lighting
    {
        enum class LightType
        {
            DIRECTIONAL,
            POINT,
        };

        class Light
        {
        public:

            Light(glm::vec3 direction, glm::vec3 colour);
            Light(glm::vec3 position, glm::vec3 color, float intensity);

            void SetLightUniforms(Shader& shader);

        private:
            LightType type;
            glm::vec3 direction;
            glm::vec3 position;
            glm::vec3 colour;

            float intensity;

        };
    }
}




