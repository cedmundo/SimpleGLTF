#include "model.h"

#include <glad/glad.h>
#define CGLTF_IMPLEMENTATION
#include "cgltf.h"

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

Model MakeCube(float dim) {
  Model model = {0};
  model.primitivesCount = 1;
  model.primitives = calloc(1, sizeof(Primitive));
  Primitive *prim = model.primitives;

  // a plane
  // clang-format off
  float vertices[] = {
    // POSITIONS
    -dim, -dim,  dim, //0
     dim, -dim,  dim, //1
    -dim,  dim,  dim, //2
     dim,  dim,  dim, //3
    -dim, -dim, -dim, //4
     dim, -dim, -dim, //5
    -dim,  dim, -dim, //6
     dim,  dim, -dim  //7
  };
  unsigned indices[] = {
    //Top
    2, 6, 7,
    2, 3, 7,

    //Bottom
    0, 4, 5,
    0, 1, 5,

    //Left
    0, 2, 6,
    0, 4, 6,

    //Right
    1, 3, 7,
    1, 5, 7,

    //Front
    0, 2, 3,
    0, 1, 3,

    //Back
    4, 6, 7,
    4, 5, 7
  };
  // clang-format on
  prim->indicesCount = 36;

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

void RenderModel(Model model, Camera camera) {
  unsigned spid = model.shader.spId;
  glUseProgram(spid);
  glBindVertexArray(model.vao);

  Mat4 viewMat = TransformGetModelMatrix(camera.transform);
  Mat4 projMat = CameraGetProjMatrix(camera);
  Mat4 modelMat = TransformGetModelMatrix(model.transform);

  int modelMat4Loc = glGetUniformLocation(spid, "model");
  int viewMat4Loc = glGetUniformLocation(spid, "view");
  int projMat4Loc = glGetUniformLocation(spid, "proj");

  glUniformMatrix4fv(modelMat4Loc, 1, GL_FALSE, Mat4Raw(&modelMat));
  glUniformMatrix4fv(viewMat4Loc, 1, GL_FALSE, Mat4Raw(&viewMat));
  glUniformMatrix4fv(projMat4Loc, 1, GL_FALSE, Mat4Raw(&projMat));
  for (int i = 0; i < model.primitivesCount; i++) {
    // IMPORTANT NOTE: Maybe assign a type GL_UNSIGNED_SHORT | GL_UNSIGNED_INT
    // in case of getting a larger type at reading model.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.primitives[i].ebo);
    glDrawElements(GL_TRIANGLES, (GLsizei)model.primitives[i].indicesCount,
                   GL_UNSIGNED_SHORT, 0);
  }
  glBindVertexArray(0);
}
