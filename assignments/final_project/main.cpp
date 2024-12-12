
#include "Bella/Mechanic/Scene/Scene.h"

// Scene must be the first thing created, as it inits OpenGL from constructor
final_project::Scene scene;

Bella_GPR200::Shader genModelShader("./assets/genericModel.vert", "./assets/genericModel.frag");
Bella_GPR200::Model testModel("./assets/Models/plant.fbx", genModelShader);

int main() {
    scene.addModel(testModel);

    scene.play();

    return 0;
}