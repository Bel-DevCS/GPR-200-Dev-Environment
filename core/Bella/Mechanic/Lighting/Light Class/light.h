//
// Created by Kearura on 2024-10-26.
//

#pragma once

#include "glm/glm.hpp"
#include "../../shader.h"

class Shader;

namespace Bella_GPR200
{
    namespace Lighting
    {
        enum class LightType
        {
            DIRECTIONAL,
            POINT,
            SPOTLIGHT,

        };

        enum class LightingModel
        {
            PHONG,
            BLINN_PHONG,
        };

        class Light
        {
        public:

            Light()
                : type(LightType::DIRECTIONAL), model(LightingModel::PHONG), direction(glm::vec3(0.0f, -1.0f, 0.0f)),
                  position(glm::vec3(0.0f)), colour(glm::vec3(1.0f)), intensity(1.0f), cutoff(12.5f),
                  AmbientK(0.1f), DiffuseK(0.5f), SpecularK(1.0f), Shininess(32.0f) {}

            static Light CreateDirectional(const glm::vec3& direction, const glm::vec3& colour,
                                          LightingModel model = LightingModel::PHONG);
            static Light CreatePoint(const glm::vec3& position, const glm::vec3& colour,
                                     float intensity, LightingModel model = LightingModel::PHONG);
            static Light CreateSpotlight(const glm::vec3& position, const glm::vec3& direction,
                float cutoff, const glm::vec3& color, float intensity, LightingModel model = LightingModel::PHONG);



            void SetLightUniforms(Shader& shader);
            void SetLightingModel(LightingModel model) { this->model = model; }
            void SetAmbientK(float k) { AmbientK = k; }
            void SetDiffuseK(float k) { DiffuseK = k; }
            void SetSpecularK(float k) { SpecularK = k; }
            void SetShininess(float s) { Shininess = s; }
            void SetColour(glm::vec3 c) { colour = c; }
            void SetPosition(const glm::vec3& newPosition) { position = newPosition; }
            void SetDirection(const glm::vec3& newDirection) { direction = newDirection; }
            void SetIntensity(const float newIntensity) { intensity = newIntensity; }
            void SetCutoff(const float newCutoff) {cutoff = newCutoff; }

            float GetAmbientK() { return AmbientK; }
            float GetDiffuseK() { return DiffuseK;}
            float GetSpecularK() { return SpecularK; }
            float GetShininess() { return Shininess; }
            float GetIntensity() const {return intensity;}
            float GetCutoff() const {return cutoff;}

            glm::vec3 GetColor() const {return colour;}
            glm::vec3 GetPosition() const {return position;}
            glm::vec3 GetDirection() const {return direction;}

            LightType GetType() const {return type;}


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
            float cutoff = 12.5f; //Spotlight

        };
    }
}




