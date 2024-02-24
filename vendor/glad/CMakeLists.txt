add_library(glad)
target_sources(glad
  INTERFACE include/glad/glad.h include/KHR/khrplatform.h
  PRIVATE src/glad.c)
target_include_directories(glad
  PUBLIC include)
