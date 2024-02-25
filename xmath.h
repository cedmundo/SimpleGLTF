#pragma once

#define DEG2RAD (0.0174533f)
#define RAD2DEG (63.662f)
#define EPSILON (0.000001f)

typedef struct {
  float x;
  float y;
} Vec2;

typedef struct {
  float x;
  float y;
  float z;
} Vec3;

typedef struct {
  float x;
  float y;
  float z;
  float w;
} Vec4;

typedef struct {
  // clang-format off
  float xx; float xy; float xz; float xw;
  float yx; float yy; float yz; float yw;
  float zx; float zy; float zz; float zw;
  float wx; float wy; float wz; float ww;
  // clang-format on
} Mat4;

typedef Vec4 Quat;
typedef Mat4 Transform;

// Return UP orientation for Vec3.
Vec3 Vec3Up();

// Return the length of a vector
float Vec3Len(Vec3 v);

// Return the squared length of a vector
float Vec3LenSq(Vec3 v);

// Normalize a vector and return the result
Vec3 Vec3Normalize(Vec3 v);

// Return the cross product between a and b
Vec3 Vec3Cross(Vec3 a, Vec3 b);

// Return the vector addition between  a and b
Vec3 Vec3Add(Vec3 a, Vec3 b);

// Return the vector difference between a and b
Vec3 Vec3Sub(Vec3 a, Vec3 b);

// Return the standard matrix multiplication of a and b
Mat4 Mat4MulMat4(Mat4 l, Mat4 r);

// Make a transform with default rotation, scale and translation.
Transform TransformIdentity();

// Move an existing transform to the offset relative to original.
Transform TransformTranslated(Transform t, Vec3 o);

// Look at a certain target rotating the transform.
Transform TransformLookAt(Transform o, Vec3 t, Vec3 up);

// Return a projection perspective for the aspect ratio.
Mat4 MakeProjPerspective(float aspectRatio, float near, float far, float fov);
