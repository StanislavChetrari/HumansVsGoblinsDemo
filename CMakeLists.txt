cmake_minimum_required(VERSION 3.10)
project(HumansVsGoblins)

function(RelativeToAbsolutePath FILE_PATH)
	if(DEFINED ${FILE_PATH})
		if(NOT IS_ABSOLUTE ${${FILE_PATH}})
			set(${FILE_PATH} ${PROJECT_SOURCE_DIR}/${${FILE_PATH}} PARENT_SCOPE)
		endif()
	endif()
endfunction()

RelativeToAbsolutePath(SDL2_INCLUDE_DIR)
RelativeToAbsolutePath(SDL2_LIB_PATH)
RelativeToAbsolutePath(SDL2_MAIN_LIB_PATH)
RelativeToAbsolutePath(SDL2_WIN_DLL_PATH)
RelativeToAbsolutePath(LIBPNG_INCLUDE_DIR)
RelativeToAbsolutePath(LIBPNG_LIB_PATH)
RelativeToAbsolutePath(LIBPNG_WIN_DLL_PATH)
RelativeToAbsolutePath(LIBPNG_ZLIP_WIN_DLL_PATH)
RelativeToAbsolutePath(ACUTEST_INCLUDE_DIR)

# This function adds a source/header file to the project. 
# SRCS_VAR   - is the variable where all source files are written
# PROJ_GROUP - group in the project. For example filters in Visual Studio or other IDE. This group folders must be separated by \\
# FILE_PATH  - file to be added
# Example call AddSrcFile(SRCS YLE\\Src Src/main.c)
function(AddFile SRCS_VAR PROJ_GROUP FILE_PATH)
	#message("source_group(${PROJ_GROUP} FILES ${FILE_PATH})")
	source_group(${PROJ_GROUP} FILES ${FILE_PATH})
	set(${SRCS_VAR} ${${SRCS_VAR}} ${FILE_PATH} PARENT_SCOPE)
endfunction()

set(PROJ_SOURCE_FILES "")

###################################################################
AddFile(PROJ_SOURCE_FILES Src Src/main.c)
###################################################################

add_subdirectory(HumansVsGoblinsEngine)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Src")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	
		add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	
		# /MP (multi-processor compilation)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14 /MP")
		
		# Set as startup project 
		set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
		
	endif()
endif()

add_executable(${PROJECT_NAME} ${PROJ_SOURCE_FILES})

# Copy resources
	configure_file("Resources/Maps/map1.map" "${PROJECT_BINARY_DIR}/Resources/Maps/map1.map" COPYONLY)
	configure_file("Resources/Maps/map1_textures/aerial_rocks_04_diff_1k.png" "${PROJECT_BINARY_DIR}/Resources/Maps/map1_textures/aerial_rocks_04_diff_1k.png" COPYONLY)
	configure_file("Resources/Maps/map1_textures/brick_floor_003_diffuse_1k.png" "${PROJECT_BINARY_DIR}/Resources/Maps/map1_textures/brick_floor_003_diffuse_1k.png" COPYONLY)
	configure_file("Resources/Maps/map1_textures/brick_wall_001_diffuse_1k.png" "${PROJECT_BINARY_DIR}/Resources/Maps/map1_textures/brick_wall_001_diffuse_1k.png" COPYONLY)
	configure_file("Resources/Maps/map1_textures/transparent_rexture_1.png" "${PROJECT_BINARY_DIR}/Resources/Maps/map1_textures/transparent_rexture_1.png" COPYONLY)
	
	configure_file("Resources/Maps/demoMapInterior.map" "${PROJECT_BINARY_DIR}/Resources/Maps/demoMapInterior.map" COPYONLY)
	configure_file("Resources/Maps/demoMapInterior_textures/brick_128x128.png" "${PROJECT_BINARY_DIR}/Resources/Maps/demoMapInterior_textures/brick_128x128.png" COPYONLY)
	configure_file("Resources/Maps/demoMapInterior_textures/brick2_128x128.png" "${PROJECT_BINARY_DIR}/Resources/Maps/demoMapInterior_textures/brick2_128x128.png" COPYONLY)
	configure_file("Resources/Maps/demoMapInterior_textures/cobble_ceiling.png" "${PROJECT_BINARY_DIR}/Resources/Maps/demoMapInterior_textures/cobble_ceiling.png" COPYONLY)
	configure_file("Resources/Maps/demoMapInterior_textures/cobble_wall.png" "${PROJECT_BINARY_DIR}/Resources/Maps/demoMapInterior_textures/cobble_wall.png" COPYONLY)
	configure_file("Resources/Maps/demoMapInterior_textures/floorTiles_128x128.png" "${PROJECT_BINARY_DIR}/Resources/Maps/demoMapInterior_textures/floorTiles_128x128.png" COPYONLY)
	configure_file("Resources/Maps/demoMapInterior_textures/floorTilesBlue_128x128.png" "${PROJECT_BINARY_DIR}/Resources/Maps/demoMapInterior_textures/floorTilesBlue_128x128.png" COPYONLY)
	configure_file("Resources/Maps/demoMapInterior_textures/rck_2.png" "${PROJECT_BINARY_DIR}/Resources/Maps/demoMapInterior_textures/rck_2.png" COPYONLY)
	configure_file("Resources/Maps/demoMapInterior_textures/tile14_128x128.png" "${PROJECT_BINARY_DIR}/Resources/Maps/demoMapInterior_textures/tile14_128x128.png" COPYONLY)
	configure_file("Resources/Maps/demoMapInterior_textures/tile21_128x128.png" "${PROJECT_BINARY_DIR}/Resources/Maps/demoMapInterior_textures/tile21_128x128.png" COPYONLY)
	configure_file("Resources/Maps/demoMapInterior_textures/bars.png" "${PROJECT_BINARY_DIR}/Resources/Maps/demoMapInterior_textures/bars.png" COPYONLY)
	
	configure_file("Resources/Maps/demoMapExterior.map" "${PROJECT_BINARY_DIR}/Resources/Maps/demoMapExterior.map" COPYONLY)
	configure_file("Resources/Maps/demoMapExterior_textures/cobble_ceiling.png" "${PROJECT_BINARY_DIR}/Resources/Maps/demoMapExterior_textures/cobble_ceiling.png" COPYONLY)
	configure_file("Resources/Maps/demoMapExterior_textures/grass_256x256.png" "${PROJECT_BINARY_DIR}/Resources/Maps/demoMapExterior_textures/grass_256x256.png" COPYONLY)
# End copy resources

# Link HumansVsGoblinsEngine
	target_link_libraries(${PROJECT_NAME} LINK_PUBLIC HumansVsGoblinsEngine)

	if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
		if(EXISTS ${PROJECT_BINARY_DIR}/HumansVsGoblinsEngine/Debug/HumansVsGoblinsEngine.dll)
			configure_file(${PROJECT_BINARY_DIR}/HumansVsGoblinsEngine/Debug/HumansVsGoblinsEngine.dll ${PROJECT_BINARY_DIR}/Debug COPYONLY)
		endif()
		if(EXISTS ${PROJECT_BINARY_DIR}/HumansVsGoblinsEngine/Release/HumansVsGoblinsEngine.dll)
			configure_file(${PROJECT_BINARY_DIR}/HumansVsGoblinsEngine/Release/HumansVsGoblinsEngine.dll ${PROJECT_BINARY_DIR}/Release COPYONLY)
		endif()
	endif()
# End link HumansVsGoblinsEngine

# Link SDL2
	if(NOT DEFINED SDL2_INCLUDE_DIR)
		message(FATAL_ERROR "SDL2 include directory not specified (SDL2_INCLUDE_DIR)!")
	endif()
	if(NOT DEFINED SDL2_LIB_PATH)
		message(FATAL_ERROR "SDL2 lib path not specified (SDL2_LIB_PATH)!")
	endif()

	get_filename_component(SDL2_LIB_DIR ${SDL2_LIB_PATH} DIRECTORY)
	get_filename_component(SDL2_LIB_FILE ${SDL2_LIB_PATH} NAME)

	include_directories(${SDL2_INCLUDE_DIR})
	target_link_directories(${PROJECT_NAME} PUBLIC "${SDL2_LIB_DIR}")
	target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIB_FILE})

	if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
		if(NOT DEFINED SDL2_MAIN_LIB_PATH)
			message(FATAL_ERROR "SDL2 main lib not specified (SDL2_MAIN_LIB_PATH)!")
		endif()

		get_filename_component(SDL2_MAIN_LIB_DIR ${SDL2_MAIN_LIB_PATH} DIRECTORY)
		get_filename_component(SDL2_MAIN_LIB_FILE ${SDL2_MAIN_LIB_PATH} NAME)

		target_link_directories(${PROJECT_NAME} PUBLIC "${SDL2_MAIN_LIB_DIR}")
		target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_MAIN_LIB_FILE})

		if(DEFINED SDL2_WIN_DLL_PATH)
			configure_file("${SDL2_WIN_DLL_PATH}" ${PROJECT_BINARY_DIR} COPYONLY)
		endif()
	endif()
# End link SDL2

# Copy libpng zlib
	if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
		if(DEFINED LIBPNG_WIN_DLL_PATH)
			configure_file("${LIBPNG_WIN_DLL_PATH}" ${PROJECT_BINARY_DIR} COPYONLY)
		endif()
		if(DEFINED LIBPNG_ZLIP_WIN_DLL_PATH)
			configure_file("${LIBPNG_ZLIP_WIN_DLL_PATH}" ${PROJECT_BINARY_DIR} COPYONLY)
		endif()
	endif()
# End copy libpng zlib
