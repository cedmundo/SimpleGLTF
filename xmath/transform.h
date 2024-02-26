#pragma once
#include "mat4.h"
#include "vec3.h"

typedef struct {
  Vec3 scale;
  Vec3 origin;
  Vec3 angles;
} Transform;

// Returns the default transform for an object
Transform MakeTransform();

// Returns the model matrix after transformations
Mat4 TransformGetModelMatrix(Transform transform);
