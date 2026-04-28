cmake_minimum_required(VERSION 3.16)
project(viewport_manager VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Collect sources
file(GLOB_RECURSE VIEWPORT_MANAGER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Create library
add_library(viewport_manager STATIC ${VIEWPORT_MANAGER_SOURCES})

# Compiler options
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	target_compile_options(viewport_manager PUBLIC -std=gnu++2b)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	target_compile_options(viewport_manager PUBLIC /std:c++latest)
endif()

# Add all src subdirectories to includes
file(GLOB SRC_SUBDIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/src/*)
set(SRC_INCLUDE_DIRS)
foreach(SUBDIR ${SRC_SUBDIRS})
	if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/${SUBDIR})
		list(APPEND SRC_INCLUDE_DIRS "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/${SUBDIR}>")
	endif()
endforeach()

target_include_directories(viewport_manager
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
		${SRC_INCLUDE_DIRS}
		$<INSTALL_INTERFACE:include>
)

# Dependencies
if(NOT TARGET raster-core)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../raster-core raster-core-build)
endif()

if(NOT TARGET render-api)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../render-api render-api-build)
endif()

if(NOT TARGET scene-core)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../scene-core scene-core-build)
endif()

target_link_libraries(viewport_manager PUBLIC raster-core render-api scene-core)

# Find SDL2
find_package(SDL2 REQUIRED)
target_link_libraries(viewport_manager PUBLIC SDL2::SDL2)

# Find Vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(viewport_manager PUBLIC Vulkan::Vulkan)
