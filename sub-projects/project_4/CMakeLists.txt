file(
        GLOB_RECURSE PROJECT4INC CONFIGURE_DEPENDS
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        *.h *.hpp
)

file(
        GLOB_RECURSE PROJECT4_SRC CONFIGURE_DEPENDS
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        *.c *.cpp
)
#Copies this assignment3's asset folder to bin when it is built
add_custom_target(copyAssetsP4 ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/)

install(FILES ${PROJECT4_INC} DESTINATION include/project4)
project(project4)
add_executable(project4 ${PROJECT4_SRC} ${PROJECT4_INC})
target_link_libraries(project4 PUBLIC core IMGUI glm assimp)
target_include_directories(project4 PUBLIC ${CORE_INC_DIR} ${stb_INCLUDE_DIR} ${assimp_INCLUDE_DIRS})

#Trigger asset copy when assignment3 is built
add_dependencies(project4 copyAssetsP4)