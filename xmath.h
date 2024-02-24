#pragma once

typedef struct {
  struct {
    float x;
    float y;
  };
} Vec2;

typedef struct {
  struct {
    float x;
    float y;
    float z;
  };
} Vec3;

typedef struct {
  struct {
    float x;
    float y;
    float z;
    float w;
  };
} Vec4;

typedef struct {
  // clang-format off
  struct {
    float xx; float xy; float xz; float xw;
    float yx; float yy; float yz; float yw;
    float zx; float zy; float zz; float zw;
    float wx; float wy; float wz; float ww;
  };
  // clang-format on
} Mat4;

typedef Vec4 Quat;
typedef Mat4 Transform;

// Return UP orientation for Vec3.
Vec3 Vec3Up();

// Make a transform with default rotation, scale and translation.
Transform TransformIdentity();

// Move an existing transform to the offset relative to original.
Transform TransformTranslated(Transform original, Vec3 offset);

// Look at a certain target rotating the transform.
Transform TransformLookAt(Transform original, Vec3 target, Vec3 up);
