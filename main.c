#include "core.h"

#define WINDOW_WIDTH 500
#define WINDOW_HEIGTH 500
#define WINDOW_TITLE "SimpleGLTF"

int main() {
  StatusCode status;

  status = AppInit(WINDOW_WIDTH, WINDOW_HEIGTH, WINDOW_TITLE);
  if (status != SUCCESS) {
    return AppClose(status);
  }

  Shader shader = LoadShader("assets/def_vs.glsl", "assets/def_fs.glsl");
  if (shader.status != SUCCESS) {
    return AppClose(shader.status);
  }

  Model model = LoadModel("assets/uwu.gltf");
  if (model.status != SUCCESS) {
    return AppClose(model.status);
  }

  model.transform = TransformIdentity();
  model.shader = shader;

  while (!AppShouldClose()) {
    BeginFrame();
    { // Render pass
      RenderModel(model);
    }
    EndFrame();
  }

  DestroyShader(shader);
  DestroyModel(model);
  return AppClose(SUCCESS);
}
