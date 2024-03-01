#include "model.h"

#include <glad/glad.h>
#define CGLTF_IMPLEMENTATION
#include "cgltf.h"

StatusCode UploadIndices(Mesh *mesh, cgltf_accessor *indices_accessor);
StatusCode UploadPositionAttribute(Mesh *mesh, cgltf_attribute attribute);

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
  model.meshesCount = 1;
  model.meshes = calloc(1, sizeof(Mesh));
  Mesh *mesh = model.meshes;

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
  mesh->indicesCount = 36;

  // upload model
  glGenVertexArrays(1, &model.vao);
  glGenBuffers(1, &mesh->vbo);
  glGenBuffers(1, &mesh->ebo);

  glBindVertexArray(model.vao);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
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

  // Bind each accessor as VertexAttrib
  unsigned vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  size_t meshesCount = 0;
  for (size_t i = 0; i < data->meshes_count; i++) {
    meshesCount += data->meshes[i].primitives_count;
  }

  Mesh *meshes = calloc(meshesCount, sizeof(Mesh));
  if (meshes == NULL) {
    model.status = E_OUT_OF_MEMORY;
    Log(LOG_ERROR, "error loading file: %s (out of memory), result: %d", path,
        result);
    return model;
  }

  size_t meshIndex = 0;
  for (size_t mi = 0; mi < data->meshes_count; mi++) {
    for (size_t pi = 0; pi < data->meshes[mi].primitives_count; pi++) {
      Mesh *mesh = meshes + meshIndex;
      cgltf_primitive primitive = data->meshes[mi].primitives[pi];

      // Gen buffers for mesh main buffer
      glGenBuffers(1, &mesh->vbo);

      // Load model attributes (position, normals, color, uvs, etc)
      for (size_t ai = 0; ai < primitive.attributes_count; ai++) {
        cgltf_attribute attribute = primitive.attributes[ai];
        if (attribute.type == cgltf_attribute_type_position) {
          model.status = UploadPositionAttribute(mesh, attribute);
          if (model.status != SUCCESS) {
            Log(LOG_WARN,
                "ignoring position attribute #%d in file %s (not a vec3 of "
                "floats)",
                ai, path);
            return model;
          }
        } else {
          Log(LOG_WARN, "ignoring attribute #%d in file %s (not supported)", ai,
              path);
        }

        break;
      }

      // Load model indices
      cgltf_accessor *indices_accessor = primitive.indices;
      model.status = UploadIndices(mesh, indices_accessor);
      if (model.status != SUCCESS) {
        Log(LOG_ERROR,
            "invalid index array in file %s (not a buffer view of scalars)",
            path);
        return model;
      }

      // Next mesh
      meshIndex++;
    }
  }

  // Collect evereything and return it so we can release it when destroying
  model.meshesCount = meshesCount;
  model.meshes = meshes;
  model.vao = vao;

  glBindVertexArray(0);
  cgltf_free(data);
  return model;
}

StatusCode UploadPositionAttribute(Mesh *mesh, cgltf_attribute attribute) {
  cgltf_accessor *attr_accessor = attribute.data;
  if (attr_accessor->component_type != cgltf_component_type_r_32f ||
      attr_accessor->type != cgltf_type_vec3) {
    return E_CANNOT_LOAD_FILE;
  }

  cgltf_buffer_view *attr_view = attr_accessor->buffer_view;
  cgltf_buffer *attr_buf = attr_view->buffer;

  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
  glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)attr_view->size,
               attr_buf->data + attr_view->offset, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
  return SUCCESS;
}

StatusCode UploadIndices(Mesh *mesh, cgltf_accessor *indices_accessor) {
  unsigned ebo = 0;
  glGenBuffers(1, &ebo);
  if (indices_accessor->type != cgltf_type_scalar) {
    return E_CANNOT_LOAD_FILE;
  }

  cgltf_buffer_view *indices_view = indices_accessor->buffer_view;
  cgltf_buffer *indices_buffer = indices_view->buffer;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)indices_view->size,
               indices_buffer->data + indices_view->offset, GL_STATIC_DRAW);

  mesh->ebo = ebo;
  mesh->indicesCount = indices_accessor->count;
  return SUCCESS;
}

void DestroyModel(Model model) {
  if (model.meshes != NULL) {
    for (int i = 0; i < model.meshesCount; i++) {
      DestroyMesh(model.meshes[i]);
    }

    free(model.meshes);
  }

  if (model.vao != 0) {
    glDeleteVertexArrays(1, &model.vao);
  }
}

void DestroyMesh(Mesh mesh) {
  if (mesh.ebo != 0) {
    glDeleteBuffers(1, &mesh.ebo);
  }

  if (mesh.vbo != 0) {
    glDeleteBuffers(1, &mesh.vbo);
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
  for (int i = 0; i < model.meshesCount; i++) {
    // IMPORTANT NOTE: Maybe assign a type GL_UNSIGNED_SHORT | GL_UNSIGNED_INT
    // in case of getting a larger type at reading model.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.meshes[i].ebo);
    glDrawElements(GL_TRIANGLES, (GLsizei)model.meshes[i].indicesCount,
                   GL_UNSIGNED_SHORT, 0);
  }
  glBindVertexArray(0);
}
