#pragma once
#include "xmath/transform.h"

#include <stdbool.h>
#include <stddef.h>

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
  E_OUT_OF_MEMORY,
  E_ERROR_COUNT,
} StatusCode;

typedef enum {
  LOG_TRACE,
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR,
  LOG_LEVEL_COUNT
} LogLevel;

// Set global log level
void AppSetLogLevel(LogLevel level);

// Display a log message in terminal
void Log(LogLevel level, const char *fmt, ...);

// Startup app window and initial setup.
StatusCode AppInit(int window_width, int window_height,
                   const char *window_title);

// Convert a status code into a message and exists the app returning the
// system status code (E_CODE).
int AppClose(StatusCode status);

// Return true if app should be closed.
bool AppShouldClose();

// Return current delta time.
float GetDeltaTime();

// Start a frame
void BeginFrame();

// Commits a frame, poll events and swap buffers
void EndFrame();

// Return the size of the main viewport in pixels
Vec2 GetViewportSize();

// Load the entire file by name into memory
char *LoadFileContents(const char *name);

// Release the contents of a file loaded into memory
void UnloadFileContents(char *data);
