#pragma once
#include "xmath.h"

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

typedef struct {
  unsigned spId;
  StatusCode status;
} Shader;

typedef struct {
  size_t indicesCount;
  unsigned vbo;
  unsigned ebo;
} Primitive;

typedef struct {
  char **buffersData;
  size_t buffersCount;
  Primitive *primitives;
  size_t primitivesCount;
  unsigned vao;

  Shader shader;
  Transform transform;
  StatusCode status;
} Model;

typedef struct {
  enum { CAMERA_PERSPECTIVE, CAMERA_ORTHO } mode;
  Transform transform;
} Camera;

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

// Start a frame
void BeginFrame();

// Commits a frame, poll events and swap buffers
void EndFrame();

// Load, compile and link a shader program using a fragment and vertex shaders.
Shader LoadShader(const char *vsPath, const char *fsPath);

// Destroy a shader if needed.
void DestroyShader(Shader shader);

// Load a GLTF model into memory decoding its data and uploading to the GPU.
Model LoadModel(const char *path);

// Destroy all contents of a model.
void DestroyModel(Model model);

// Delete all related buffer arrays of primitive
void DestroyPrimitive(Primitive primitive);

// Display a model in the GPU
void RenderModel(Model model);

// Return a perspective camera looking at origin offset a little bit
Camera MakeDefaultCamera();

// Load the entire file by name into memory
char *LoadFileContents(const char *name);

// Release the contents of a file loaded into memory
void UnloadFileContents(char *data);
