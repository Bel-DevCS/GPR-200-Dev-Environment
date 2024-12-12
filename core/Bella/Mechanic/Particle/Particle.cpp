#include "Particle.h"

namespace Bella_GPR200
{
    Particle::Particle()
    : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f), Size(1.0f) {}

    void Particle::Update(float deltaTime)
    {
        if (Life > 0.0f) {
            Life -= deltaTime; // Reduce life over time
            Position += Velocity * deltaTime; // Update position
            Color.a -= deltaTime * 0.5f; // Gradually fade out
            if (Color.a < 0.0f) Color.a = 0.0f; // Clamp alpha to 0
        }
    }

    void Particle::Reset(const glm::vec3& position, const glm::vec3& velocity, const glm::vec4& color, float lifespan, float size)
    {
        Position = position;
        Velocity = velocity;
        Color = color;
        Life = lifespan;
        Size = size;
    }

    const glm::vec3& Particle::GetPosition() const
    {
        return Position;
    }

    const glm::vec4& Particle::GetColor() const
    {
        return Color;
    }

    float Particle::GetSize() const
    {
        return Size;
    }

    bool Particle::IsAlive() const
    {
        return Life > 0.0f;
    }
}

