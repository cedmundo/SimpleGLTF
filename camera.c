#include "camera.h"

#include "core.h"

Mat4 CameraGetProjMatrix(Camera camera) {
  assert((camera.mode == CAMERA_MODE_PERSPECTIVE_PROJ ||
          camera.mode == CAMERA_MODE_ORTHO_PROJ) &&
         "invalid state: unknown camera mode");
  if (camera.mode == CAMERA_MODE_PERSPECTIVE_PROJ) {
    return Mat4MakePerspective(camera.fov, camera.aspect, camera.near,
                               camera.far);
  }

  // FIXME(cedmundo): we might need to use -camera.width/2 and camera.width/2
  return Mat4MakeOrtho(0, camera.width, 0, camera.height, camera.near,
                       camera.far);
}

Camera MakeDefaultCamera() {
  Camera camera = {0};
  camera.mode = CAMERA_MODE_PERSPECTIVE_PROJ;
  camera.transform = MakeTransform();
  camera.transform.origin = (Vec3){0.0f, 0.0f, -10.0f};
  camera.front = Vec3Forward;
  camera.up = Vec3Up;
  camera.fov = CAMERA_DEFAULT_FOV;
  camera.near = CAMERA_DEFAULT_NEAR;
  camera.far = CAMERA_DEFAULT_FAR;
  return camera;
}

void UpdateCamera(Camera *camera) {
  Vec2 viewportSize = GetViewportSize();
  camera->aspect = viewportSize.x / viewportSize.y;
  camera->width = viewportSize.x;
  camera->height = viewportSize.y;
}
