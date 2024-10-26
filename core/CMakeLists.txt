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
        Bella/Mechanic/shader.h
        Bella/Mechanic/shader.cpp
        Bella/Mechanic/texture.h
        Bella/Definitions/definitionFunctions.h
        Bella/Definitions/drawShape.h
        Bella/Definitions/definitionColours.h
        Bella/Mechanic/camera.h
        Bella/Mechanic/Model/mesh.cpp
        Bella/Mechanic/Model/mesh.h
        Bella/Mechanic/Model/model.cpp
        Bella/Mechanic/Model/model.h
        Bella/Mechanic/light.cpp
        Bella/Mechanic/light.h
)

find_package(OpenGL REQUIRED)

target_link_libraries(core PUBLIC IMGUI glm)

install (TARGETS core DESTINATION lib)
install (FILES ${CORE_INC} DESTINATION include/core)

