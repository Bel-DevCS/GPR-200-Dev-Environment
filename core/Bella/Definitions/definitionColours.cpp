//
// Created by Cura on 2024-10-15.
//

#include "definitionColours.h"

namespace Bella_GPR200
{
    glm::vec4* Colour::RandomColour(int numVertices)
    {
        glm::vec4* colours = new glm::vec4[numVertices];
        for (int i = 0; i < numVertices; i++)
        {
            colours[i] = glm::vec4((float)rand() / RAND_MAX,  // Random red
                                   (float)rand() / RAND_MAX,  // Random green
                                   (float)rand() / RAND_MAX,  // Random blue
                                   1.0f);                    // Alpha is 1.0
        }
        return colours;
    }
}