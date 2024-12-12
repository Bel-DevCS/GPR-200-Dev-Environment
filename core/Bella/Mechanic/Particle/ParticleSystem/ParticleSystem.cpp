#include "ParticleSystem.h"
#include <glm/ext/matrix_transform.hpp>

namespace Bella_GPR200
{
    ParticleSystem::ParticleSystem(size_t maxParticles, const Bella_GPR200::Shader& shader)
    : maxParticles(maxParticles), shader(shader) {
    particles.resize(maxParticles); // Initialize particle pool
    InitializeBuffers();
}

void ParticleSystem::InitializeBuffers() {
    float particleQuad[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
         0.5f,  0.5f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particleQuad), particleQuad, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

    void ParticleSystem::Update(float deltaTime, const glm::vec3& emitterPosition, float terrainWidth, float terrainDepth) {
        for (auto& particle : particles) {
            particle.Update(deltaTime);

            if (!particle.IsAlive()) {
                // Spawn particle within the terrain bounds
                float xOffset = static_cast<float>(rand()) / RAND_MAX * terrainWidth - terrainWidth / 2.0f;
                float zOffset = static_cast<float>(rand()) / RAND_MAX * terrainDepth - terrainDepth / 2.0f;

                glm::vec3 position = emitterPosition + glm::vec3(xOffset, 0.0f, zOffset);
                SpawnParticle(position);
            }
        }
    }

    void ParticleSystem::Render(const glm::mat4& view, const glm::mat4& projection) {
        shader.use();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glBindVertexArray(VAO);

        for (const auto& particle : particles) {
            if (particle.IsAlive()) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, particle.GetPosition());
                model = glm::rotate(model, static_cast<float>(rand()) / RAND_MAX * glm::two_pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f)); // Random rotation
                model = glm::scale(model, glm::vec3(particle.GetSize()));

                shader.setMat4("model", model);
                shader.setVec4("color", particle.GetColor());

                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            }
        }

        glBindVertexArray(0);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }


    void ParticleSystem::SetConfig(const ParticleConfig& config) {
        this->config = config;
    }

    void ParticleSystem::SpawnParticle(const glm::vec3& position) {
        size_t unusedParticle = FindUnusedParticle();
        glm::vec3 velocity = RandomVector(config.velocityMin, config.velocityMax);

        // Introduce slight clumping
        glm::vec3 clumpOffset = glm::vec3(
            ((rand() % 100) - 50) / 500.0f,
            0.0f,
            ((rand() % 100) - 50) / 500.0f
        );

        glm::vec4 color = config.colorStart;
        float lifespan = config.lifespanMin + static_cast<float>(rand()) / RAND_MAX * (config.lifespanMax - config.lifespanMin);
        float size = config.sizeMin + static_cast<float>(rand()) / RAND_MAX * (config.sizeMax - config.sizeMin);

        particles[unusedParticle].Reset(position + clumpOffset, velocity, color, lifespan, size);
    }


    glm::vec3 ParticleSystem::RandomVector(const glm::vec3& min, const glm::vec3& max) {
        return glm::vec3(
            min.x + static_cast<float>(rand()) / RAND_MAX * (max.x - min.x),
            min.y + static_cast<float>(rand()) / RAND_MAX * (max.y - min.y),
            min.z + static_cast<float>(rand()) / RAND_MAX * (max.z - min.z)
        );
    }



size_t ParticleSystem::FindUnusedParticle() {
    for (size_t i = 0; i < particles.size(); ++i) {
        if (!particles[i].IsAlive()) {
            return i;
        }
    }
    return 0; // Reuse the first particle if none are available
}
}

