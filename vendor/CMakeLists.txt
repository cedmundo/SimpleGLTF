# GLFW
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Disable shared library")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Disable examples")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Disable tests")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Disable docs")
set(GLFW_INSTALL OFF CACHE BOOL "Disable install")
add_subdirectory(glfw)

# GLAD
add_subdirectory(glad)

# cGLTF
add_library(cgltf INTERFACE)
target_include_directories(cgltf INTERFACE cgltf)
