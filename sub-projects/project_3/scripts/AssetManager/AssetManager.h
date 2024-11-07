//
// Created by Cura on 2024-11-05.
//

#pragma once
#include "Bella/Mechanic/camera.h"

#include <map>
#include <string>
#include <stdexcept>

class AssetManager
{
    public:
        void AddCamera(const std::string& name, const Bella_GPR200::Camera& camera)
        {
            cameras[name] = camera;
        }

    Bella_GPR200::Camera& GetCamera(const std::string& name)
    {
        if (cameras.find(name) == cameras.end()) {
            throw std::runtime_error("Camera with name '" + name + "' not found");
        }
        return cameras.at(name);
    }

    private:
        std::map<std::string, Bella_GPR200::Camera> cameras;
};