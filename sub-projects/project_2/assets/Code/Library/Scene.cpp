#include "Scene.h"

namespace Bella_Engine {

    void Scene::Initialize() {
        bgColour = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);

        // Initialize the active camera
        if (activeCamera) {
            activeCamera->Position = glm::vec3(3.0f, 0.0f, 0.0f);  // Move the camera closer
            activeCamera->Initialize();
        }

        // Set initial model position and scale
        for (auto& model : models) {
            model->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));  // Position at origin
            model->SetScale(glm::vec3(1.0f));                 // Normal scale
            model->SetRotation(glm::vec3(0.0f));              // No rotation
            model->Initialize();
        }

        // Initialize lights
        for (auto& light : lights) {
            light->Initialize();
        }
    }

    void Scene::Update(float deltaTime) {
        // Update the active camera with deltaTime
        if (activeCamera) {
            activeCamera->UpdateCameraVectors(deltaTime);
        }

        // Update all models in the scene
        for (auto& model : models) {
            model->Update(deltaTime);
        }
    }


void Scene::Render() {
    // Clear the screen with the background color
    glClearColor(bgColour.r, bgColour.g, bgColour.b, bgColour.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Ensure there is an active camera
    if (activeCamera) {
        // Get camera matrices and position
        glm::mat4 view = activeCamera->GetViewMatrix();
        glm::mat4 projection = activeCamera->GetProjectionMatrix();
        glm::vec3 viewPos = activeCamera->Position;

        // Loop through each model in the scene
        for (auto& model : models) {
            std::shared_ptr<Shader> shader = model->GetShader();
            if (shader) {
                shader->use();  // Activate the shader

                // Set camera uniforms
                shader->setMat4("view", view);
                shader->setMat4("projection", projection);
                shader->setVec3("viewPos", viewPos);

                // Set the model transformation matrix
                glm::mat4 modelMatrix = glm::mat4(1.0f);
                modelMatrix = glm::translate(modelMatrix, model->GetPosition());
                modelMatrix = glm::scale(modelMatrix, model->GetScale());
                shader->setMat4("model", modelMatrix);

                // Lighting uniforms
                if (!lights.empty()) {
                    auto light = lights.front();  // Use the first light for demonstration
                    shader->setVec3("lightColor", light->GetColor());
                    shader->setFloat("lightIntensity", light->GetIntensity());
                    shader->setInt("lightType", static_cast<int>(light->GetType()));

                    // Directional light
                    if (light->GetType() == LightType::DIRECTIONAL) {
                        shader->setVec3("lightDir", light->GetDirection());
                    }

                    // Point/spotlight position
                    if (light->GetType() == LightType::POINT || light->GetType() == LightType::SPOTLIGHT) {
                        shader->setVec3("lightPos", light->GetPosition());
                    }

                    // Spotlight-specific properties
                    if (light->GetType() == LightType::SPOTLIGHT) {
                        shader->setVec3("lightDir", light->GetDirection());
                        shader->setFloat("cutoff", glm::cos(glm::radians(light->GetCutoff())));
                    }

                    // Material properties
                    shader->setFloat("AmbientK", light->GetAmbientK());
                    shader->setFloat("DiffuseK", light->GetDiffuseK());
                    shader->setFloat("SpecularK", light->GetSpecularK());
                    shader->setFloat("Shininess", light->GetShininess());
                    shader->setInt("lightingModel", static_cast<int>(light->GetLightingModel()));
                }

                // Draw the model (this function handles mesh rendering)
                model->Draw(*shader);
            }
        }
    }
}




    void Scene::Cleanup() {
        // Cleanup all models in the scene
        for (auto& model : models) {
            model->Cleanup();
            std::cout << "Model cleaned up." << std::endl;
        }

        // Cleanup all lights in the scene
        for (auto& light : lights) {
            light->Cleanup();
            std::cout << "Light cleaned up." << std::endl;
        }

        // Cleanup the active camera
        if (activeCamera) {
            activeCamera->Cleanup();
            std::cout << "Active camera cleaned up." << std::endl;
        }
    }

    void Scene::AddObject(const std::shared_ptr<Model>& model) {
        models.push_back(model);
        std::cout << "Model added to scene." << std::endl;
    }

    void Scene::AddLight(const std::shared_ptr<Light>& light) {
        lights.push_back(light);
        std::cout << "Light added to scene." << std::endl;
    }

    void Scene::SetActiveCamera(const std::shared_ptr<Camera>& camera) {
        activeCamera = camera;
        std::cout << "Active camera set." << std::endl;
    }
}
