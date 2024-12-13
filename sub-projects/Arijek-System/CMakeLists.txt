file(
 GLOB_RECURSE ARIJEK_INC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.h *.hpp
)

file(
 GLOB_RECURSE ARIJEK_SRC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.c *.cpp
)
#Copies this assignment1's asset folder to bin when it is built
add_custom_target(copyAssetsARIJEK ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_CURRENT_SOURCE_DIR}/assets/
${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/)

install(FILES ${ARIJEK_INC} DESTINATION include/ARIJEK)
project(ARIJEK)
add_executable(ARIJEK ${ARIJEK_SRC} ${ARIJEK_INC}
        "assets/Code/Scene Manager/SceneManager.cpp"
        "assets/Code/Scene Manager/SceneManager.h"
        "assets/Code/User Input/UserInput.cpp"
        "assets/Code/User Input/UserInput.h")
target_link_libraries(ARIJEK PUBLIC core IMGUI glm assimp)
target_include_directories(ARIJEK PUBLIC ${CORE_INC_DIR} ${stb_INCLUDE_DIR} ${assimp_INCLUDE_DIRS})

#Trigger asset copy when assignment1 is built
add_dependencies(ARIJEK copyAssetsARIJEK)