
#include "Bella/Mechanic/Scene/Scene.h"

#define MAX_SNOW_PARTICLES 1000

// Scene must be the first thing created, as it inits OpenGL from constructor
final_project::Scene scene;

Bella_GPR200::Shader genModelShader("./assets/Shaders/genericModel.vert", "./assets/Shaders/genericModel.frag");
Bella_GPR200::Model testModel("./assets/Models/plant.fbx", genModelShader);

Bella_GPR200::Model modelTwo("./assets/Models/plant.fbx", genModelShader);


// Create a Point Light
Bella_GPR200::Lighting::Light pointLight = Bella_GPR200::Lighting::Light::CreatePoint(
        glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3(1.0f, 0.8f, 0.6f), 1.0f
);

//Particles
Bella_GPR200::Shader snowShader("assets/Shaders/Particles/Snow/SnowVert.vert", "assets/Shaders/Particles/Snow/SnowFrag.frag");
Bella_GPR200::ParticleSystem snowParticleGenerator(MAX_SNOW_PARTICLES,snowShader);

int main() {
    Bella_GPR200::ParticleConfig snowConfig;
    snowConfig.velocityMin = glm::vec3(-0.1f, -0.5f, -0.1f);
    snowConfig.velocityMax = glm::vec3(0.1f, -0.5f, 0.1f);
    snowConfig.colorStart = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    snowConfig.colorEnd = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
    snowConfig.lifespanMin = 5.0f;
    snowConfig.lifespanMax = 7.0f;
    snowConfig.sizeMin = 0.05f;
    snowConfig.sizeMax = 0.1f;

    snowParticleGenerator.setPosition(glm::vec3 (0.0f, 10.0f, 0.0f));

    snowParticleGenerator.SetConfig(snowConfig);

    testModel.setPosition(glm::vec3(1.0f, 0.5f, -1.0f));

    scene.addModel(testModel);
    scene.addModel(modelTwo);
    scene.addLight(pointLight);

    scene.addParticleGenerator(snowParticleGenerator);

    scene.play();

    return 0;
}