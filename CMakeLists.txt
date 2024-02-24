cmake_minimum_required(VERSION 3.22)
project(SimpleGLTF
  DESCRIPTION "A simple GLTF loader and viewer"
  HOMEPAGE_URL "https://github.com/cedmundo/SimpleGLTF"
  VERSION "1.0.0"
  LANGUAGES C
)

# Global configuration
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_definitions("-Wall" "-g")

# Add dependencies
add_subdirectory(vendor)

# Add main executable
add_executable(SimpleGLTF)
target_sources(SimpleGLTF
  INTERFACE core.h
  PRIVATE core.c main.c
)
target_link_libraries(SimpleGLTF glfw glad)

# Copy assets dir
set(ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
if(EXISTS "${ASSETS_DIR}")
  add_custom_command(
    TARGET SimpleGLTF PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_CURRENT_BINARY_DIR}/assets")

  add_custom_command(
    TARGET SimpleGLTF PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${ASSETS_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/assets")
endif()
