//
// Created by Kearura on 2024-10-26.
//

#include "light.h"

namespace Bella_GPR200
{
    namespace Lighting
    {

        Light Light::CreateDirectional(const glm::vec3& direction, const glm::vec3& colour, LightingModel model)
        {
            Light light;
            light.type = LightType::DIRECTIONAL;
            light.direction = direction;
            light.colour = colour;
            light.model = model;
            return light;
        }

        Light Light::CreatePoint(const glm::vec3& position, const glm::vec3& colour, float intensity, LightingModel model)
        {
            Light light;
            light.type = LightType::POINT;
            light.position = position;
            light.colour = colour;
            light.intensity = intensity;
            light.model = model;
            return light;
        }

        Light Light::CreateSpotlight(const glm::vec3& position, const glm::vec3& direction, float cutoff, const glm::vec3& color, float intensity, LightingModel model)
        {
            Light light;
            light.type = LightType::SPOTLIGHT;
            light.position = position;
            light.direction = direction;
            light.cutoff = cutoff;
            light.colour = color;
            light.intensity = intensity;
            light.model = model;
            return light;
        }


        void Light::SetLightUniforms(Shader& shader)
        {
            shader.setInt("lightType", static_cast<int>(type));

            shader.setFloat("AmbientK", AmbientK);
            shader.setFloat("DiffuseK", DiffuseK);
            shader.setFloat("SpecularK", SpecularK);
            shader.setFloat("Shininess", Shininess);

            switch(type)
            {
                case LightType::DIRECTIONAL:
                    shader.setVec3("lightDir", direction);
                    shader.setVec3("lightColor", colour);
                    shader.setFloat("lightIntensity", 1.0f);
                break;

                case LightType::POINT:
                    shader.setVec3("lightPos", position);
                    shader.setVec3("lightColor", colour);
                    shader.setFloat("lightIntensity", intensity);
                break;

            case LightType::SPOTLIGHT:
                shader.setVec3("lightPos", position);
                shader.setVec3("lightDir", direction);
                shader.setVec3("lightColor", colour);
                shader.setFloat("lightIntensity", intensity);
                shader.setFloat("cutoff", glm::cos(glm::radians(cutoff)));
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