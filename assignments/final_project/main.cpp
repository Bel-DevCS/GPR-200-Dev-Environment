
#include "Bella/Mechanic/Scene/Scene.h"

// Scene must be the first thing created, as it inits OpenGL from constructor
final_project::Scene scene;

Bella_GPR200::Shader genModelShader("./assets/genericModel.vert", "./assets/genericModel.frag");
Bella_GPR200::Model testModel("./assets/Models/plant.fbx", genModelShader);

Bella_GPR200::Model modelTwo("./assets/Models/plant.fbx", genModelShader);


// Create a Point Light
Bella_GPR200::Lighting::Light pointLight = Bella_GPR200::Lighting::Light::CreatePoint(
        glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3(1.0f, 0.8f, 0.6f), 1.0f
);

int main() {
    testModel.setPosition(glm::vec3(1.0f, 0.5f, -1.0f));

    scene.addModel(testModel);
    scene.addModel(modelTwo);
    scene.addLight(pointLight);

    scene.play();

    return 0;
}