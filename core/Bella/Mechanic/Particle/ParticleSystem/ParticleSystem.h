#pragma once

#include "../Particle.h"       // Include the Particle class
#include <vector>              // To manage a collection of particles
#include <glm/glm.hpp>         // For vector and matrix math
#include "../../Shader/shader.h" // Include the Shader class

namespace Bella_GPR200 {
    // ParticleSystem class manages a pool of particles and handles their updates and rendering
    class ParticleSystem {
    public:
        // Constructor: Initialize with a maximum number of particles and a shader
        ParticleSystem(size_t maxParticles, const Shader& shader);

        // Set configuration for particle behavior
        void SetConfig(const ParticleConfig& config);

        // Update particles based on time and terrain bounds
        void Update(float deltaTime, const glm::vec3& emitterPosition, float terrainWidth, float terrainDepth);

        // Render all active particles
        void Render(const glm::mat4& view, const glm::mat4& projection);

        // Spawn a new particle at a specific position
        void SpawnParticle(const glm::vec3& position);

    private:
        std::vector<Particle> particles; // Pool of particles
        size_t maxParticles;             // Maximum number of particles
        Shader shader;                   // Shader used for rendering particles
        ParticleConfig config;           // Configuration for particle properties

        unsigned int VAO, VBO;           // OpenGL buffers for the particle quad

        size_t FindUnusedParticle();     // Find an unused particle in the pool
        void InitializeBuffers();        // Initialize OpenGL buffers for rendering
        glm::vec3 RandomVector(const glm::vec3& min, const glm::vec3& max); // Generate random vector within a range
    };
}
