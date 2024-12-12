#include "Particle.h"

namespace Bella_GPR200
{
    // Constructor: Initializes a default particle with neutral properties
    Particle::Particle()
        : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f), Size(1.0f) {}

    // Update the particle's state over time
    void Particle::Update(float deltaTime) {
        if (Life > 0.0f) {
            // Decrease the particle's remaining life
            Life -= deltaTime;

            // Simulate wind effect by adding a directional offset
            glm::vec3 wind(0.2f, 0.0f, 0.1f);
            Position += (Velocity + wind) * deltaTime;

            // Gradually fade out the particle
            Color.a -= deltaTime * 0.5f;
            if (Color.a < 0.0f) Color.a = 0.0f; // Ensure alpha doesn't go below 0
        }
    }

    // Reset the particle's properties for reuse
    void Particle::Reset(const glm::vec3& position, const glm::vec3& velocity, const glm::vec4& color, float lifespan, float size)
    {
        Position = position;   // Set the particle's starting position
        Velocity = velocity;   // Set the particle's velocity
        Color = color;         // Set the particle's starting color
        Life = lifespan;       // Set the particle's lifespan
        Size = size;           // Set the particle's size
    }

    // Get the particle's position
    const glm::vec3& Particle::GetPosition() const
    {
        return Position;
    }

    // Get the particle's color
    const glm::vec4& Particle::GetColor() const
    {
        return Color;
    }

    // Get the particle's size
    float Particle::GetSize() const
    {
        return Size;
    }

    // Check if the particle is still active (life > 0)
    bool Particle::IsAlive() const
    {
        return Life > 0.0f;
    }
}
