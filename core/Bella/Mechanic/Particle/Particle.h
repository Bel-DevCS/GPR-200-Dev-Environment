//
// Created by Kearura on 2024-12-12.
//

#pragma once

#include <glm/glm.hpp>

namespace Bella_GPR200
{
    struct ParticleConfig
    {
        glm::vec3 velocityMin = glm::vec3(-0.1f, -1.0f, -0.1f);
        glm::vec3 velocityMax = glm::vec3(0.1f, -1.0f, 0.1f);
        glm::vec4 colorStart = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // Start color
        glm::vec4 colorEnd = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);   // End color
        float lifespanMin = 2.0f;
        float lifespanMax = 5.0f;
        float sizeMin = 0.1f;
        float sizeMax = 0.3f;
    };

    class Particle
    {
    public:
        Particle();

        // Update the particle state (position, color, etc.)
        void Update(float deltaTime);

        // Reset the particle for reuse
        void Reset(const glm::vec3& position, const glm::vec3& velocity, const glm::vec4& color, float lifespan, float size);

        // Accessors
        const glm::vec3& GetPosition() const;
        const glm::vec4& GetColor() const;
        float GetSize() const;
        bool IsAlive() const;

    private:
        glm::vec3 Position;   // Current position of the particle
        glm::vec3 Velocity;   // Current velocity of the particle
        glm::vec4 Color;      // RGBA color of the particle
        float Life;           // Remaining life of the particle
        float Size;           // Size of the particle
    };
}
