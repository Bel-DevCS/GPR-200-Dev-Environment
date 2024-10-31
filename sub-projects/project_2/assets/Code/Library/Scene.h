//
// Created by Kearura on 2024-10-27.
//


#pragma once
#include "ew/external/glad.h"
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <list>
#include <memory>
#include <glm/vec4.hpp>

#include "../sub-projects/project_2/assets/Code/Objects/camera.h"
#include "../sub-projects/project_2/assets/Code/Objects/light.h"
#include "../sub-projects/project_2/assets/Code/Objects/Model/model.h"



namespace Bella_Engine
{
    class Camera;
    class Light;
    class Model;

    class Scene
    {
    public:
        void Initialize();
        void Update(float deltaTime);
        void Render();
        void Cleanup();

        void AddObject(const std::shared_ptr<Model>& model);
        void AddLight(const std::shared_ptr<Light>& light);
        void SetActiveCamera(const std::shared_ptr<Camera>& camera);

        std::shared_ptr<Camera> GetActiveCamera() const {return activeCamera;}
        std::list<std::shared_ptr<Light>> GetLights() const {return lights;}
        std::list<std::shared_ptr<Model>> GetModels() const {return models;}

    private:
        std::list<std::shared_ptr<Model>> models;  // Models in the scene
        std::list<std::shared_ptr<Light>> lights;  // Lights in the scene
        std::shared_ptr<Camera> activeCamera;      // Active camera


        glm::vec4 bgColour;
    };
}
