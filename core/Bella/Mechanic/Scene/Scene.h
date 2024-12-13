//
// Created by anne.konicki on 11/12/2024.
//

#ifndef SCENE_H
#define SCENE_H
#include <vector>
#include <string>

#include "../../../ew/external/glad.h"
#include "./../../../ak/Camera.h"
#include "../Model/model.h"
#include "../Lighting/Light Class/light.h"
#include "../Terrain/TerrainGenerator.h"
#include "../Particle/ParticleSystem/ParticleSystem.h"

namespace final_project {

class Scene {
public:
    Scene();

    // Play the scene once all configuration has completed.
    void play();

    // Add a model to the scene
    void addModel(Bella_GPR200::Model& model) { mModels.push_back(model); }
    // Add a light source to the scene
    void addLight(Bella_GPR200::Lighting::Light& light) { mLights.push_back(light); }
    // Add a particle particleGenerator to the scene
    void addParticleGenerator(Bella_GPR200::ParticleSystem& particleGenerator) { mParticleGenerators.push_back(particleGenerator); }

    // Set the terrain generator for the scene
    void setTerrain(TerrainGenerator& terrainGenerator) { mTerrainGenerator = terrainGenerator; }

private:
    // Initialize OpenGL and the scene Window
    void initialize();

    // Draw UI from ImGUI
    void drawUI();

    // Container for all models within the scene
    std::vector<Bella_GPR200::Model> mModels;

    // Container for all lights within the scene
    std::vector<Bella_GPR200::Lighting::Light> mLights;

    // Container for all particle generators in a scene
    std::vector<Bella_GPR200::ParticleSystem> mParticleGenerators;

    // Terrain Generator for the scene
    TerrainGenerator mTerrainGenerator = TerrainGenerator(0, 0, 0);

    // The scene's camera
    ak::Camera mCamera;
    // A pointer to the window the scene will render to
    GLFWwindow* mWindow = nullptr;

    static constexpr unsigned int SCREEN_WIDTH = 1080;
    static constexpr unsigned int SCREEN_HEIGHT = 720;
    static constexpr float NEAR_PLANE = 0.1f;
    static constexpr float FAR_PLANE = 1000.0f;

    const std::string mSCENE_NAME = "Scene";

    // Default screen color
    glm::vec3 mClearScreenColor = glm::vec3(0.3, 0.4, 0.9);
    // All bits to clear on frame draw
    int mClearBit = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

    // Callback method for mouse scrolling
    static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
    // Callback method for mouse movement
    static void mouse_callback(GLFWwindow* window, double xPos, double yPos);
};

} // final_project

#endif //SCENE_H
