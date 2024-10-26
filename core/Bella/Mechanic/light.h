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

        enum class LightingModel
        {
            PHONG,
            BLINN_PHONG,
        };

        class Light
        {
        public:

            Light(glm::vec3 direction, glm::vec3 colour, LightingModel model = LightingModel::PHONG);
            Light(glm::vec3 position, glm::vec3 color, float intensity, LightingModel model = LightingModel::PHONG);

            void SetLightUniforms(Shader& shader);
            void SetLightingModel(LightingModel model) { this->model = model; }
            void SetAmbientK(float k) { AmbientK = k; }
            void SetDiffuseK(float k) { DiffuseK = k; }
            void SetSpecularK(float k) { SpecularK = k; }
            void SetShininess(float s) { Shininess = s; }

        private:
            LightType type;
            LightingModel model;
            glm::vec3 direction;
            glm::vec3 position;
            glm::vec3 colour;

            float intensity;

            float AmbientK = 0.1f;
            float DiffuseK = 0.5f;
            float SpecularK = 1.0f;
            float Shininess = 32.0f;

        };
    }
}




