#include "core.h"

// Standard libraries
#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// GLAD / GLFW
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

typedef struct {
  bool didInitGLFW;
  float deltaTime;
  double curFrame;
  double lstFrame;
  float fpsTime;
  int fpsCount;
  int curFPS;
  float windowWidth;
  float windowHeight;
  GLFWwindow *window;
  LogLevel logLevel;
} App;

static App app = {0};

StatusCode AppInit(int window_width, int window_height,
                   const char *window_title) {
  // Init GLFW.
  if (!glfwInit()) {
    return E_CANNOT_INIT_GLFW;
  }

  // Allow GLFW termination
  app.didInitGLFW = true;

  // Window hints for OpenGL.
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create window.
  app.window =
      glfwCreateWindow(window_width, window_height, window_title, NULL, NULL);
  if (app.window == NULL) {
    return E_CANNOT_CREATE_WINDOW;
  }

  // Without this gladLoadGL will fail.
  glfwMakeContextCurrent(app.window);
  if (!gladLoadGL()) {
    return E_CANNOT_LOAD_GL;
  }

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  return SUCCESS;
}

void AppSetLogLevel(LogLevel level) { app.logLevel = level; }

void Log(LogLevel level, const char *fmt, ...) {
  assert(level >= LOG_TRACE && level < LOG_LEVEL_COUNT &&
         "invalid arg: level must be in LogLevel range");
  if (level < app.logLevel) {
    return;
  }

  static const char *prefixes[] = {
      [LOG_TRACE] = "TRACE",
      [LOG_INFO] = "INFO",
      [LOG_WARN] = "WARN",
      [LOG_ERROR] = "ERROR",
  };

  FILE *logOutput = stdout;
  if (level == LOG_ERROR) {
    logOutput = stderr;
  }

  fprintf(logOutput, "%s: ", prefixes[level]);

  va_list valist;
  va_start(valist, fmt);
  vfprintf(logOutput, fmt, valist);
  va_end(valist);

  fprintf(logOutput, "\n");
}

int AppClose(StatusCode status) {
  assert(status >= SUCCESS && status < E_ERROR_COUNT &&
         "invalid arg status: outside range");

  if (app.window != NULL) {
    glfwDestroyWindow(app.window);
  }

  if (app.didInitGLFW) {
    glfwTerminate();
  }

  if (status != SUCCESS && errno != 0) {
    assert(fprintf(stderr, "error: %s \n", strerror(errno)) > 0);
    return -1;
  }

  return 0;
}

bool AppShouldClose() {
  assert(app.window != NULL && "invalid state: app.window is not initialized");
  return glfwWindowShouldClose(app.window);
}

float GetDeltaTime() { return app.deltaTime; }

void BeginFrame() {
  assert(app.window != NULL && "invalid state: app.window is not initialized");
  int width = 0;
  int height = 0;

  // Framebuffer resize and aspect ratio
  glfwGetFramebufferSize(app.window, &width, &height);
  app.windowWidth = (float)width;
  app.windowHeight = (float)height;

  // Uptime and delta time
  app.curFrame = glfwGetTime();
  app.deltaTime = (float)(app.curFrame - app.lstFrame);
  app.lstFrame = app.curFrame;

  // FPS Counting
  app.fpsTime += app.deltaTime;
  app.fpsCount += 1;
  if (app.fpsTime >= 1.0f) {
    app.curFPS = app.fpsCount;
    app.fpsCount = 0;
    app.fpsTime = 0.0f;
  }

  // Clear buffer and start frame
  glViewport(0, 0, width, height);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void EndFrame() {
  assert(app.window != NULL && "invalid state: app.window is not initialized");
  glfwPollEvents();
  glfwSwapBuffers(app.window);
}

Vec2 GetViewportSize() {
  return (Vec2){.x = app.windowWidth, .y = app.windowHeight};
}

char *LoadFileContents(const char *name) {
  assert(name != NULL && "invalid arg name: cannot be NULL");
  FILE *file = fopen(name, "re");
  if (file == NULL) {
    return NULL;
  }

  size_t file_size = 0L;
  assert(fseek(file, 0, SEEK_END) == 0);
  file_size = ftell(file);
  assert(fseek(file, 0, SEEK_SET) == 0);

  char *data = calloc(sizeof(char), file_size);
  size_t read_size = fread(data, sizeof(char), file_size, file);
  if (read_size == 0) {
    assert(fclose(file) == 0);
    free(data);
    return NULL;
  }

  return data;
}

void UnloadFileContents(char *data) {
  assert(data != NULL && "invalid arg data: cannot be NULL");
  if (data != NULL) {
    free(data);
  }
}
