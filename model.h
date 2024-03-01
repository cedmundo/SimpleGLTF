#pragma once
#include <stddef.h>
#include <xmath/transform.h>

#include "camera.h"
#include "core.h"

// Shader holds the program id after loading
typedef struct {
  unsigned spId;
  StatusCode status;
} Shader;

// A single vertex representing the attributes required by the shader
typedef struct {
  Vec3 pos;
  Vec3 nor;
  Vec2 uvs;
  Vec4 col;
} Vertex;

// Primitive reflects a single mesh instance of a model
typedef struct {
  Vertex *vertices;
  size_t verticesCount;
  size_t indicesCount;
  unsigned vbo;
  unsigned ebo;
} Mesh;

// Model wraps a mesh with a material, a transform and its buffers.
typedef struct {
  Mesh *meshes;
  size_t meshesCount;
  unsigned vao;

  Shader shader;
  Transform transform;
  StatusCode status;
} Model;

// Load, compile and link a shader program using a fragment and vertex shaders.
Shader LoadShader(const char *vsPath, const char *fsPath);

// Destroy a shader if needed.
void DestroyShader(Shader shader);

// Make a single plane
Model MakeCube(float dim);

// Load a GLTF model into memory decoding its data and uploading to the GPU.
Model LoadModel(const char *path);

// Destroy all contents of a model.
void DestroyModel(Model model);

// Delete all related buffer arrays of mesh
void DestroyMesh(Mesh mesh);

// Render a model from the point of view of given camera
void RenderModel(Model model, Camera camera);
