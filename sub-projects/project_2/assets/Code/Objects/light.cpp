#include "light.h"

namespace Bella_Engine {

    Light Light::CreateDirectional(const glm::vec3& direction, const glm::vec3& colour, LightingModel model) {
        Light light;
        light.type = LightType::DIRECTIONAL;
        light.direction = direction;
        light.colour = colour;
        light.model = model;
        return light;
    }

    Light Light::CreatePoint(const glm::vec3& position, const glm::vec3& colour, float intensity, LightingModel model) {
        Light light;
        light.type = LightType::POINT;
        light.position = position;
        light.colour = colour;
        light.intensity = intensity;
        light.model = model;
        return light;
    }

    Light Light::CreateSpotlight(const glm::vec3& position, const glm::vec3& direction, float cutoff,
                                 const glm::vec3& color, float intensity, LightingModel model) {
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

    void Light::Initialize() {
        // Initialization logic, if needed
        std::cout << "Initializing light of type: " << static_cast<int>(type) << std::endl;
    }

    void Light::RenderLight(Shader& shader, int lightIndex) {
        // Sets up the light's uniforms for the shader
        SetLightUniforms(shader, lightIndex);
    }

    void Light::SetLightUniforms(Shader& shader, int lightIndex) {
        std::string index = std::to_string(lightIndex);
        std::string lightPrefix = "lights[" + index + "].";

        shader.setInt(lightPrefix + "type", static_cast<int>(type));
        shader.setVec3(lightPrefix + "color", colour);
        shader.setFloat(lightPrefix + "intensity", intensity);
        shader.setFloat(lightPrefix + "ambientK", AmbientK);
        shader.setFloat(lightPrefix + "diffuseK", DiffuseK);
        shader.setFloat(lightPrefix + "specularK", SpecularK);
        shader.setFloat(lightPrefix + "shininess", Shininess);

        switch (type) {
            case LightType::DIRECTIONAL:
                shader.setVec3(lightPrefix + "direction", direction);
                break;
            case LightType::POINT:
                shader.setVec3(lightPrefix + "position", position);
                break;
            case LightType::SPOTLIGHT:
                shader.setVec3(lightPrefix + "position", position);
                shader.setVec3(lightPrefix + "direction", direction);
                shader.setFloat(lightPrefix + "cutoff", glm::cos(glm::radians(cutoff)));
                break;
        }

        switch (model) {
            case LightingModel::PHONG:
                shader.setInt(lightPrefix + "lightingModel", 0); // 0 for Phong
                break;
            case LightingModel::BLINN_PHONG:
                shader.setInt(lightPrefix + "lightingModel", 1); // 1 for Blinn-Phong
                break;
        }
    }

    void Light::Cleanup() {
        // Reset light attributes, if necessary
        std::cout << "Cleaning up light of type: " << static_cast<int>(type) << std::endl;
        type = LightType::DIRECTIONAL;
        model = LightingModel::PHONG;
        direction = glm::vec3(0.0f, -1.0f, 0.0f);
        position = glm::vec3(0.0f);
        colour = glm::vec3(1.0f);
        intensity = 1.0f;
        cutoff = 12.5f;
        AmbientK = 0.1f;
        DiffuseK = 0.5f;
        SpecularK = 1.0f;
        Shininess = 32.0f;
    }
}
