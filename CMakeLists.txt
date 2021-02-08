cmake_minimum_required(VERSION 3.16)

project(Gemcutter LANGUAGES C CXX CSharp)

find_package(OpenGL REQUIRED)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/scaffold")
include(scaffold)
sf_is_project_top_level(IS_TOP_LEVEL)

add_subdirectory(external)
add_subdirectory(gemcutter)
add_subdirectory(tools)

option(ENABLE_TESTS "Build unit tests." ${IS_TOP_LEVEL})
if (ENABLE_TESTS)
	add_subdirectory(tests)
endif()

option(ENABLE_SAMPLES "Download and build sample projects." ${IS_TOP_LEVEL})
if (ENABLE_SAMPLES)
	sf_clone_external_git_repo(
		URL "https://github.com/EmilianC/Gemcutter-Samples.git"
		TARGET_DIR "${CMAKE_CURRENT_SOURCE_DIR}/samples"
		TAG CMake
		ALWAYS_UPDATE FALSE
	)

	add_subdirectory(samples)
endif()
