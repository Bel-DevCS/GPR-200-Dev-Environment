#include "Bella/Mechanic/shader.h"

#include "scripts/Program.h"
#include "scripts/AssetManager/AssetManager.h"
#include "scripts/InitiateAssets.h"

Program program;


int main()
{
    program.Rise();
    InitiateAssets(program.assetManager);
    program.RenderLoop();
    program.Terminate();

    return 0;
}
