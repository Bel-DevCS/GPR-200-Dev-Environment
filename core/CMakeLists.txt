file(
 GLOB_RECURSE CORE_INC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.h *.hpp
)

file(
 GLOB_RECURSE CORE_SRC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.c *.cpp
)

set(CORE_INC_DIR
 ${CMAKE_CURRENT_SOURCE_DIR}
 CACHE PATH "CORE INCLUDE SOURCE PATH"
)

add_library(core STATIC ${CORE_SRC} ${CORE_INC}
        Bella/Mechanic/Shader/shader.h
        Bella/Mechanic/Shader/shader.cpp
        Bella/Mechanic/Texture/texture.h
        "Bella/Definitions/Misc Functions/definitionFunctions.h"
        Bella/Definitions/Shapes/Vertex/drawShape.h
        Bella/Definitions/Colours/definitionColours.h
        Bella/Mechanic/Camera/camera.h
        Bella/Mechanic/Particle/Particle.cpp
        Bella/Mechanic/Particle/Particle.h
        Bella/Mechanic/Particle/ParticleSystem/ParticleSystem.cpp
        Bella/Mechanic/Particle/ParticleSystem/ParticleSystem.h
        Bella/Mechanic/Scene/scene.cpp
        Bella/Mechanic/Scene/scene.h
        ak/Camera.h

)

find_package(OpenGL REQUIRED)

target_link_libraries(core PUBLIC IMGUI glm)

install (TARGETS core DESTINATION lib)
install (FILES ${CORE_INC} DESTINATION include/core)

