file(
 GLOB_RECURSE FINAL_PROJECT_INC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.h *.hpp
)

file(
 GLOB_RECURSE FINAL_PROJECT_SRC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.c *.cpp
)
#Copies this project's asset folder to bin when it is built
add_custom_target(copyAssetsFP ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_CURRENT_SOURCE_DIR}/assets/
${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/)

install(FILES ${FINAL_PROJECT_INC} DESTINATION include/final_project)
project(final_project)
add_executable(final_project ${FINAL_PROJECT_SRC} ${FINAL_PROJECT_INC})
target_link_libraries(final_project PUBLIC core IMGUI glm assimp)
target_include_directories(final_project PUBLIC ${CORE_INC_DIR} ${stb_INCLUDE_DIR} ${assimp_INCLUDE_DIRS})

#Trigger asset copy when assignment4 is built
add_dependencies(final_project copyAssetsA5)