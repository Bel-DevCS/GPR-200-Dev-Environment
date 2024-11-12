file(
        GLOB_RECURSE PROJECT3INC CONFIGURE_DEPENDS
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        *.h *.hpp
)

file(
        GLOB_RECURSE PROJECT3_SRC CONFIGURE_DEPENDS
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        *.c *.cpp
)
#Copies this assignment2's asset folder to bin when it is built
add_custom_target(copyAssetsP3 ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/)

install(FILES ${PROJECT3_INC} DESTINATION include/project2)
project(project3)
add_executable(project3 ${PROJECT3_SRC} ${PROJECT3_INC})
target_link_libraries(project3 PUBLIC core IMGUI glm assimp)
target_include_directories(project3 PUBLIC ${CORE_INC_DIR} ${stb_INCLUDE_DIR} ${assimp_INCLUDE_DIRS})

#Trigger asset copy when assignment2 is built
add_dependencies(project3 copyAssetsP3)