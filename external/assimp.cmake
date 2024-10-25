#ASSIMP
string(TIMESTAMP BEFORE "%s")

CPMAddPackage(
	NAME "assimp"
	URL "https://github.com/assimp/assimp/archive/refs/tags/v5.2.5.zip"
	OPTIONS (
		"ASSIMP_BUILD_SAMPLES OFF" "ASSIMP_BUILD_ZLIB ON" "BUILD_SHARED_LIBS OFF"
		"ASSIMP_BUILD_TESTS OFF"
		"ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF"
		"ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF"
		"ASSIMP_BUILD_FBX_IMPORTER ON"
		"ASSIMP_BUILD_OBJ_IMPORTER ON"
		"ASSIMP_BUILD_COLLADA_IMPORTER ON"

		"ASSIMP_WARNINGS_AS_ERRORS OFF" #modified to prevent warnings from stopping program execution
	)
)

find_package(assimp REQUIRED)
set(assimp_INCLUDE_DIRS
		"${assimp_SOURCE_DIR}/include"
		"${assimp_BINARY_DIR}/include" #modified to fix
)

include_directories(
		"${assimp_SOURCE_DIR}/include"
		"${assimp_BINARY_DIR}/include" #modified to fix
)

link_directories(${assimp_BINARY_DIR}/lib)

string(TIMESTAMP AFTER "%s")
math(EXPR DELTAassimp "${AFTER}-${BEFORE}")
MESSAGE(STATUS "assimp TIME: ${DELTAassimp}s")
