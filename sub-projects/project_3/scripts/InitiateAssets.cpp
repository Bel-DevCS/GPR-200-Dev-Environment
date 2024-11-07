//
// Created by Cura on 2024-11-05.
//

#include "InitiateAssets.h"

void InitiateAssets(AssetManager& assetManager)
{
    // Initialize and add cameras
    Bella_GPR200::Camera mainCamera(glm::vec3(0.0f, 0.0f, 1.0f));
    assetManager.AddCamera("Main Camera", mainCamera);

}