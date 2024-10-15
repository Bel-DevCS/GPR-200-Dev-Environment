//
// Created by Cura on 2024-10-15.
//

#include "definitionFunctions.h"

namespace Bella_GPR200
{
    void DefinedFunctions::debug_PrintMatrix(const glm::mat4& matrix)
    {
        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++) {
                std::cout << matrix[col][row] << " ";
            }
            std::cout << std::endl;
        }
    }
}