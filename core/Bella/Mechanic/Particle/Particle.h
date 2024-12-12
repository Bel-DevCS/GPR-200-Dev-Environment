#pragma once

#include <glm/glm.hpp>

namespace Bella_GPR200
{
    // Configuration struct to define the properties of a particle system
    struct ParticleConfig
    {
        glm::vec3 velocityMin = glm::vec3(-0.1f, -1.0f, -0.1f); // Minimum velocity of a particle
        glm::vec3 velocityMax = glm::vec3(0.1f, -1.0f, 0.1f);   // Maximum velocity of a particle
        glm::vec4 colorStart = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // Initial color (white, opaque)
        glm::vec4 colorEnd = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);   // Final color (white, transparent)
        float lifespanMin = 2.0f;  // Minimum lifespan of a particle (seconds)
        float lifespanMax = 5.0f;  // Maximum lifespan of a particle (seconds)
        float sizeMin = 0.1f;      // Minimum size of a particle
        float sizeMax = 0.3f;      // Maximum size of a particle
    };

    // Class to represent an individual particle
    class Particle
    {
    public:
        Particle();

        // Update the particle's state based on time elapsed
        void Update(float deltaTime);

        // Reset the particle's properties for reuse
        void Reset(const glm::vec3& position, const glm::vec3& velocity, const glm::vec4& color, float lifespan, float size);

        // Accessors for particle properties
        const glm::vec3& GetPosition() const; // Get particle's position
        const glm::vec4& GetColor() const;    // Get particle's color
        float GetSize() const;                // Get particle's size
        bool IsAlive() const;                 // Check if the particle is still active

    private:
        glm::vec3 Position;   // Current position of the particle
        glm::vec3 Velocity;   // Velocity influencing particle movement
        glm::vec4 Color;      // Current RGBA color of the particle
        float Life;           // Remaining lifespan of the particle
        float Size;           // Current size of the particle
    };
}
