#pragma once
#include <stdbool.h>

typedef enum {
  SUCCESS,
  E_CANNOT_INIT_GLFW,
  E_CANNOT_CREATE_WINDOW,
  E_CANNOT_LOAD_GL,
  E_CANNOT_LOAD_FILE,
  E_SHADER_COMPILE_ERROR,
  E_SHADER_LINK_ERROR,
  E_CANNOT_CREATE_TEXTURE,
  E_CANNOT_UPLOAD_TEXTURE,
  E_ERROR_COUNT,
} StatusCode;

// Startup app window and initial setup.
StatusCode AppInit(int window_width, int window_height,
                   const char *window_title);

// Convert a status code into a message and exists the app returning the
// system status code (E_CODE).
int AppClose(StatusCode status);

// Return true if app should be closed.
bool AppShouldClose();

// Start a frame
void BeginFrame();

// Commits a frame, poll events and swap buffers
void EndFrame();
