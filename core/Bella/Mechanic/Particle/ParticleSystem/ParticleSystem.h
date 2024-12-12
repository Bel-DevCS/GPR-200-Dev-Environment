#pragma once

#include "../Particle.h"
#include <vector>

#include <glm/glm.hpp>
#include "../../Shader/shader.h"

namespace Bella_GPR200 {
    class ParticleSystem {
    public:
        ParticleSystem(size_t maxParticles, const Shader& shader);

        void SetConfig(const ParticleConfig& config); // Set system-wide configuration
        void Update(float deltaTime, const glm::vec3& emitterPosition);
        void Render(const glm::mat4& view, const glm::mat4& projection);
        void SpawnParticle(const glm::vec3& position);

    private:
        std::vector<Particle> particles;
        size_t maxParticles;
        Shader shader;
        ParticleConfig config; // System-wide configuration

        unsigned int VAO, VBO;
        size_t FindUnusedParticle();
        void InitializeBuffers();
        glm::vec3 RandomVector(const glm::vec3& min, const glm::vec3& max);
    };
}
