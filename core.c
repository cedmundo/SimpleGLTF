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

#define CGLTF_IMPLEMENTATION
#include "cgltf.h"

typedef struct {
  bool didInitGLFW;
  float aspectRatio;
  float deltaTime;
  double curFrame;
  double lstFrame;
  float fpsTime;
  int fpsCount;
  int curFPS;
  GLFWwindow *window;
  Camera currentCamera;
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
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

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

  app.currentCamera =
      MakeDefaultCamera((float)window_width / (float)window_height);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
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

void BeginFrame() {
  assert(app.window != NULL && "invalid state: app.window is not initialized");
  int width = 0;
  int height = 0;

  // Framebuffer resize and aspect ratio
  glfwGetFramebufferSize(app.window, &width, &height);
  app.aspectRatio = (float)width / (float)height;

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
  glClear(GL_COLOR_BUFFER_BIT);
}

void EndFrame() {
  assert(app.window != NULL && "invalid state: app.window is not initialized");
  glfwPollEvents();
  glfwSwapBuffers(app.window);
}

Shader LoadShader(const char *vsPath, const char *fsPath) {
  Shader shader = {0};
  int glStatus = 0;
  char *vsSource = NULL;
  char *fsSource = NULL;
  unsigned vsId = 0;
  unsigned fsId = 0;
  char shaderLog[512] = {0};

  vsSource = LoadFileContents(vsPath);
  if (vsSource == NULL) {
    Log(LOG_ERROR, "cannot load shader file: %s", vsPath);
    shader.status = E_CANNOT_LOAD_FILE;
    goto terminate;
  }

  fsSource = LoadFileContents(fsPath);
  if (fsSource == NULL) {
    Log(LOG_ERROR, "cannot load shader file: %s", fsPath);
    shader.status = E_CANNOT_LOAD_FILE;
    goto terminate;
  }

  glStatus = 0;
  vsId = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vsId, 1, (const char **)&vsSource, NULL);
  glCompileShader(vsId);
  glGetShaderiv(vsId, GL_COMPILE_STATUS, &glStatus);
  if (!glStatus) {
    shader.status = E_SHADER_COMPILE_ERROR;
    glGetShaderInfoLog(vsId, 512, NULL, shaderLog);
    Log(LOG_ERROR, "cannot compile vertex shader: %s: %s", vsPath, shaderLog);
    goto terminate;
  }

  glStatus = 0;
  fsId = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fsId, 1, (const char **)&fsSource, NULL);
  glCompileShader(fsId);
  glGetShaderiv(fsId, GL_COMPILE_STATUS, &glStatus);
  if (!glStatus) {
    shader.status = E_SHADER_COMPILE_ERROR;
    glGetShaderInfoLog(fsId, 512, NULL, shaderLog);
    Log(LOG_ERROR, "cannot compile fragment shader: %s: %s", fsPath, shaderLog);
    goto terminate;
  }

  shader.spId = glCreateProgram();
  glAttachShader(shader.spId, vsId);
  glAttachShader(shader.spId, fsId);
  glLinkProgram(shader.spId);
  glGetShaderiv(shader.spId, GL_LINK_STATUS, &glStatus);
  if (!glStatus) {
    shader.status = E_SHADER_LINK_ERROR;
    glGetShaderInfoLog(shader.spId, 512, NULL, shaderLog);
    Log(LOG_ERROR, "cannot link shader program: %s", shaderLog);
    goto terminate;
  }
  shader.status = SUCCESS;

terminate:
  if (vsId != 0) {
    glDetachShader(shader.spId, vsId);
    glDeleteShader(vsId);
  }

  if (fsId != 0) {
    glDetachShader(shader.spId, fsId);
    glDeleteShader(fsId);
  }

  if (shader.status != SUCCESS) {
    DestroyShader(shader);
  }

  if (vsSource != NULL) {
    free(vsSource);
  }

  if (fsSource != NULL) {
    free(fsSource);
  }

  return shader;
}

void DestroyShader(Shader shader) {
  if (shader.spId != 0) {
    glDeleteProgram(shader.spId);
  }
}

Model MakePlane(float dim) {
  Model model = {0};
  model.primitivesCount = 1;
  model.primitives = calloc(1, sizeof(Primitive));
  Primitive *prim = model.primitives;

  // a plane
  // clang-format off
  float vertices[] = {
      // POSITIONS
      dim,  dim,  0.0f,
      dim,  -dim, 0.0f,
      -dim, -dim, 0.0f,
      -dim, dim,  0.0f,
  };
  // clang-format on
  unsigned indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3, // second triangle
  };
  prim->indicesCount = 6;

  // upload model
  glGenVertexArrays(1, &model.vao);
  glGenBuffers(1, &prim->vbo);
  glGenBuffers(1, &prim->ebo);

  glBindVertexArray(model.vao);
  glBindBuffer(GL_ARRAY_BUFFER, prim->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, prim->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  return model;
}

Model LoadModel(const char *path) {
  Model model = {0};

  cgltf_options options = {0};
  cgltf_data *data = NULL;
  cgltf_result result;

  // Open file, validate its contents and load external buffers if needed
  result = cgltf_parse_file(&options, path, &data);
  if (result != cgltf_result_success) {
    model.status = E_CANNOT_LOAD_FILE;
    Log(LOG_ERROR, "could not load model: %s, result: %d", path, result);
    return model;
  }

  result = cgltf_validate(data);
  if (result != cgltf_result_success) {
    model.status = E_CANNOT_LOAD_FILE;
    Log(LOG_ERROR, "invalid model: %s, result: %d", path, result);
    return model;
  }

  result = cgltf_load_buffers(&options, data, path);
  if (result != cgltf_result_success) {
    model.status = E_CANNOT_LOAD_FILE;
    Log(LOG_ERROR, "error loading buffers of file: %s, result: %d", path,
        result);
    return model;
  }

  // Copy buffer data into internal structure in Model so we can bind them later
  size_t buffersCount = data->buffers_count;
  char **buffersData = calloc(buffersCount, sizeof(char *));
  if (buffersData == NULL) {
    model.status = E_OUT_OF_MEMORY;
    Log(LOG_ERROR, "error loading file: %s (out of memory), result: %d", path,
        result);
    return model;
  }

  for (size_t i = 0; i < buffersCount; i++) {
    cgltf_buffer buffer = data->buffers[i];
    buffersData[i] = calloc(buffer.size, sizeof(char));
    assert(memcpy(buffersData[i], buffer.data, buffer.size) == buffersData[i]);
  }

  // Bind each accessor as VertexAttrib
  unsigned vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  size_t primitivesCount = 0;
  for (size_t i = 0; i < data->meshes_count; i++) {
    primitivesCount += data->meshes[i].primitives_count;
  }

  Primitive *destPrimitives = calloc(primitivesCount, sizeof(Primitive));
  if (destPrimitives == NULL) {
    model.status = E_OUT_OF_MEMORY;
    Log(LOG_ERROR, "error loading file: %s (out of memory), result: %d", path,
        result);
    return model;
  }

  size_t destPrimitiveIndex = 0;
  for (size_t mi = 0; mi < data->meshes_count; mi++) {
    for (size_t pi = 0; pi < data->meshes[mi].primitives_count; pi++) {
      cgltf_primitive srcPrimitive = data->meshes[mi].primitives[pi];

      unsigned vbo = 0;
      glGenBuffers(1, &vbo);

      // Load model attributes (position, normals, color, uvs, etc)
      for (size_t ai = 0; ai < srcPrimitive.attributes_count; ai++) {
        cgltf_attribute attribute = srcPrimitive.attributes[ai];
        if (attribute.type != cgltf_attribute_type_position) {
          Log(LOG_WARN, "ignoring attribute #%d in file %s (not position)", ai,
              path);
          continue;
        }

        cgltf_accessor *attr_accessor = attribute.data;
        if (attr_accessor->component_type != cgltf_component_type_r_32f ||
            attr_accessor->type != cgltf_type_vec3) {
          Log(LOG_WARN,
              "ignoring attribute #%d in file %s (not a vec3 of floats)", ai,
              path);
          continue;
        }

        cgltf_buffer_view *attr_view = attr_accessor->buffer_view;
        cgltf_buffer *attr_buf = attr_view->buffer;
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)attr_view->size,
                     attr_buf->data + attr_view->offset, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
        break;
      }

      // Load model indices
      unsigned ebo = 0;
      glGenBuffers(1, &ebo);
      cgltf_accessor *indices_accessor = srcPrimitive.indices;
      if (indices_accessor->type != cgltf_type_scalar) {
        model.status = E_CANNOT_LOAD_FILE;
        Log(LOG_ERROR,
            "invalid index array in file %s (not a buffer view of scalars)",
            path);
        return model;
      }

      cgltf_buffer_view *indices_view = indices_accessor->buffer_view;
      cgltf_buffer *indices_buffer = indices_view->buffer;
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)indices_view->size,
                   indices_buffer->data + indices_view->offset, GL_STATIC_DRAW);

      // bind arrays for each primitive
      destPrimitives[destPrimitiveIndex].vbo = vbo;
      destPrimitives[destPrimitiveIndex].ebo = ebo;
      destPrimitives[destPrimitiveIndex].indicesCount = indices_accessor->count;
      destPrimitiveIndex += 1;
    }
  }

  // Collect evereything and return it so we can release it when destroying
  model.buffersCount = buffersCount;
  model.buffersData = buffersData;
  model.primitivesCount = primitivesCount;
  model.primitives = destPrimitives;
  model.vao = vao;

  glBindVertexArray(0);
  cgltf_free(data);
  return model;
}

void DestroyModel(Model model) {
  if (model.primitives != NULL) {
    for (int i = 0; i < model.primitivesCount; i++) {
      DestroyPrimitive(model.primitives[i]);
    }

    free(model.primitives);
  }

  if (model.buffersData != NULL) {
    for (int i = 0; i < model.buffersCount; i++) {
      free(model.buffersData[i]);
    }

    free(model.buffersData);
  }

  if (model.vao != 0) {
    glDeleteVertexArrays(1, &model.vao);
  }
}

void DestroyPrimitive(Primitive primitive) {
  if (primitive.ebo != 0) {
    glDeleteBuffers(1, &primitive.ebo);
  }

  if (primitive.vbo != 0) {
    glDeleteBuffers(1, &primitive.vbo);
  }
}

void RenderModel(Model model) {
  unsigned spid = model.shader.spId;
  glUseProgram(spid);
  glBindVertexArray(model.vao);

  int modelMat4Loc = glGetUniformLocation(spid, "model");
  int viewMat4Loc = glGetUniformLocation(spid, "view");
  int projMat4Loc = glGetUniformLocation(spid, "proj");

  glUniformMatrix4fv(modelMat4Loc, 1, GL_FALSE, (GLfloat *)&model.transform);
  glUniformMatrix4fv(viewMat4Loc, 1, GL_FALSE,
                     (GLfloat *)&app.currentCamera.view);
  glUniformMatrix4fv(projMat4Loc, 1, GL_FALSE,
                     (GLfloat *)&app.currentCamera.proj);

  for (int i = 0; i < model.primitivesCount; i++) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.primitives[i].ebo);
    glDrawElements(GL_TRIANGLES, (GLsizei)model.primitives[i].indicesCount,
                   GL_UNSIGNED_INT, 0);
  }
  glBindVertexArray(0);
}

Camera MakeDefaultCamera(float aspectRatio) {
  Vec3 origin = (Vec3){0.0f, 0.0f, 5.0f};
  Vec3 center = (Vec3){0.0f, 0.0f, 0.0f};
  Transform view = TransformIdentity();
  view = TransformTranslated(view, origin);
  view = TransformLookAt(view, center, Vec3Up());
  Mat4 proj = MakeProjPerspective(aspectRatio, CAMERA_DEFAULT_NEAR,
                                  CAMERA_DEFAULT_FAR, CAMERA_DEFAULT_FOV);

  return (Camera){
      .mode = CAMERA_PERSPECTIVE,
      .near = CAMERA_DEFAULT_NEAR,
      .far = CAMERA_DEFAULT_FAR,
      .fov = CAMERA_DEFAULT_FOV,
      .proj = proj,
      .view = view,
  };
}

void SetCurrentCamera(Camera camera) { app.currentCamera = camera; }

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
