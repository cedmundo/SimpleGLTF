add_library(xmath OBJECT)
target_sources(xmath
  INTERFACE scalar.h vec2.h vec3.h vec4.h mat4.h transform.h
  PRIVATE scalar.c vec2.c vec3.c vec4.c mat4.c transform.c
)
target_link_libraries(xmath m)
target_include_directories(xmath
  PUBLIC ${CMAKE_SOURCE_DIR})
