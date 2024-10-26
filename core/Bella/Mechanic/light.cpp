//
// Created by Kearura on 2024-10-26.
//

#include "light.h"

namespace Bella_GPR200
{
    namespace Lighting
    {
        Light::Light(glm::vec3 direction, glm::vec3 colour, LightingModel model) : direction(direction), colour(colour), type(LightType::DIRECTIONAL), model(model) {}

        Light::Light(glm::vec3 position, glm::vec3 color, float intensity, LightingModel model) : position(position), colour(color), intensity(intensity), type(LightType::POINT), model(model) {}


        Light Light::CreateDirectional(const glm::vec3& direction, const glm::vec3& colour, LightingModel model)
        {
            return Light(direction, colour, model);
        }

        Light Light::CreatePoint(const glm::vec3& position, const glm::vec3& colour, float intensity, LightingModel model)
        {
            return Light(position, colour, intensity, model);
        }



        void Light::SetLightUniforms(Shader& shader)
        {
            shader.setFloat("AmbientK", AmbientK);
            shader.setFloat("DiffuseK", DiffuseK);
            shader.setFloat("SpecularK", SpecularK);
            shader.setFloat("Shininess", Shininess);

            switch(type)
            {
                case LightType::DIRECTIONAL:
                    shader.setVec3("lightDir", direction);
                    shader.setVec3("lightColor", colour);
                break;

                case LightType::POINT:
                    shader.setVec3("lightPos", position);
                    shader.setVec3("lightColor", colour);
                    shader.setFloat("lightIntensity", intensity);
                break;
            }

            switch (model)
            {
            case LightingModel::PHONG:
                shader.setInt("lightingModel", 0); // 0 for Phong
                break;
            case LightingModel::BLINN_PHONG:
                shader.setInt("lightingModel", 1); // 1 for Blinn-Phong
                break;
            }
        }



    }
}