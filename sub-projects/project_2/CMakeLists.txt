file(
        GLOB_RECURSE PROJECT2INC CONFIGURE_DEPENDS
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        *.h *.hpp
)

file(
        GLOB_RECURSE PROJECT2_SRC CONFIGURE_DEPENDS
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        *.c *.cpp
)
#Copies this assignment2's asset folder to bin when it is built
add_custom_target(copyAssetsP2 ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/)

install(FILES ${PROJECT2_INC} DESTINATION include/project2)
project(project2)
add_executable(project2 ${PROJECT2_SRC} ${PROJECT2_INC}
        assets/Code/Library/Scene.cpp
        assets/Code/Library/Scene.h
        assets/Code/Library/WindowManager.cpp
        assets/Code/Library/WindowManager.h
        assets/Code/Objects/camera.cpp)
target_link_libraries(project2 PUBLIC core IMGUI glm assimp)
target_include_directories(project2 PUBLIC ${CORE_INC_DIR} ${stb_INCLUDE_DIR} ${assimp_INCLUDE_DIRS})

#Trigger asset copy when assignment2 is built
add_dependencies(project2 copyAssetsP2)