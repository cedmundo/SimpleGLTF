#pragma once
#include <xmath/transform.h>
#include <xmath/vec3.h>

#define CAMERA_DEFAULT_NEAR 0.1f
#define CAMERA_DEFAULT_FAR 100.0f
#define CAMERA_DEFAULT_FOV 45.0f

// Camera represents an usable view for the render
typedef struct {
  enum {
    CAMERA_MODE_ORTHO_PROJ,
    CAMERA_MODE_PERSPECTIVE_PROJ,
  } mode;
  Vec3 front;
  Vec3 up;
  Vec3 right;
  Vec3 worldUp;

  float fov;
  float width;
  float height;
  float aspect;
  float near;
  float far;

  Transform transform;
} Camera;

// Return a perspective camera looking at origin offset a little bit
Camera MakeDefaultCamera();

// Return the *updated* projection matrix of a camera.
Mat4 CameraGetProjMatrix(Camera camera);

// Updates the current camera using core state
void UpdateCamera(Camera *camera);
