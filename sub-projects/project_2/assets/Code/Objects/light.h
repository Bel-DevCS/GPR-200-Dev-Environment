#pragma once
#include "ew/external/glad.h"
#include <glm/glm.hpp>
#include "shader.h"

namespace Bella_Engine
{

    enum class LightType
    {
        DIRECTIONAL,
        POINT,
        SPOTLIGHT
    };

    enum class LightingModel
    {
        PHONG,
        BLINN_PHONG
    };

    class Light {
    public:
        Light()
            : type(LightType::DIRECTIONAL), model(LightingModel::PHONG), direction(glm::vec3(0.0f, -1.0f, 0.0f)),
              position(glm::vec3(0.0f)), colour(glm::vec3(1.0f)), intensity(1.0f), cutoff(12.5f),
              AmbientK(0.1f), DiffuseK(0.5f), SpecularK(1.0f), Shininess(32.0f) {}

        // Static methods for light creation
        static Light CreateDirectional(const glm::vec3& direction, const glm::vec3& colour,
                                       LightingModel model = LightingModel::PHONG);
        static Light CreatePoint(const glm::vec3& position, const glm::vec3& colour,
                                 float intensity, LightingModel model = LightingModel::PHONG);
        static Light CreateSpotlight(const glm::vec3& position, const glm::vec3& direction,
                                     float cutoff, const glm::vec3& color, float intensity,
                                     LightingModel model = LightingModel::PHONG);

        // Core functions
        void Initialize();
        void RenderLight(Shader& shader, int lightIndex = 0); // Renders the light to the shader
        void SetLightUniforms(Shader& shader, int lightIndex = 0); // Sets light uniforms in the shader
        void Cleanup(); // Cleans up light resources

        // Setters and Getters (Unchanged)
        void SetLightingModel(LightingModel model) { this->model = model; }
        void SetAmbientK(float k) { AmbientK = k; }
        void SetDiffuseK(float k) { DiffuseK = k; }
        void SetSpecularK(float k) { SpecularK = k; }
        void SetShininess(float s) { Shininess = s; }
        void SetColour(glm::vec3 c) { colour = c; }
        void SetPosition(const glm::vec3& newPosition) { position = newPosition; }
        void SetDirection(const glm::vec3& newDirection) { direction = newDirection; }
        void SetIntensity(float newIntensity) { intensity = newIntensity; }
        void SetCutoff(float newCutoff) { cutoff = newCutoff; }

        LightingModel GetLightingModel() const {return model;}
        float GetAmbientK() const { return AmbientK; }
        float GetDiffuseK() const { return DiffuseK; }
        float GetSpecularK() const { return SpecularK; }
        float GetShininess() const { return Shininess; }
        float GetIntensity() const { return intensity; }
        float GetCutoff() const { return cutoff; }

        glm::vec3 GetColor() const { return colour; }
        glm::vec3 GetPosition() const { return position; }
        glm::vec3 GetDirection() const { return direction; }

        LightType GetType() const { return type; }

    private:
        LightType type;
        LightingModel model;
        glm::vec3 direction;
        glm::vec3 position;
        glm::vec3 colour;

        float intensity;
        float AmbientK;
        float DiffuseK;
        float SpecularK;
        float Shininess;
        float cutoff; // Spotlight
    };
}
