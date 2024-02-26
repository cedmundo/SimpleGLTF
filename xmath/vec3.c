#include "vec3.h"

Vec3 Vec3Make(float x, float y, float z) {
  Vec3 r;
  r.x = x;
  r.y = y;
  r.z = z;
  return r;
}

Vec3 Vec3MakeVz(Vec2 v, float z) {
  Vec3 r;
  r.x = v.x;
  r.y = v.y;
  r.z = z;
  return r;
}

Vec3 Vec3Add(const Vec3 a, const Vec3 b) {
  Vec3 r;
  r.x = a.x + b.x;
  r.y = a.y + b.y;
  r.z = a.z + b.z;
  return r;
}

Vec3 Vec3Sub(const Vec3 a, const Vec3 b) {
  Vec3 r;
  r.x = a.x - b.x;
  r.y = a.y - b.y;
  r.z = a.z - b.z;
  return r;
}

Vec3 Vec3Scale(const Vec3 a, const float s) {
  Vec3 r;
  r.x = a.x * s;
  r.y = a.y * s;
  r.z = a.z * s;
  return r;
}

float Vec3MulInner(const Vec3 a, const Vec3 b) {
  float r = 0.0f;
  r += a.x * b.x;
  r += a.y * b.y;
  r += a.z * b.z;
  return r;
}

float Vec3SqrLen(const Vec3 a) { return Vec3MulInner(a, a); }

float Vec3Len(const Vec3 a) { return sqrtf(Vec3SqrLen(a)); }

Vec3 Vec3Norm(const Vec3 a) {
  float k = 1.0f / Vec3Len(a);
  return Vec3Scale(a, k);
}

Vec3 Vec3Min(const Vec3 a, const Vec3 b) {
  Vec3 r;
  r.x = a.x < b.x ? a.x : b.x;
  r.y = a.y < b.y ? a.y : b.y;
  r.z = a.z < b.z ? a.z : b.z;
  return r;
}

Vec3 Vec3Max(const Vec3 a, const Vec3 b) {
  Vec3 r;
  r.x = a.x > b.x ? a.x : b.x;
  r.y = a.y > b.y ? a.y : b.y;
  r.z = a.z > b.z ? a.z : b.z;
  return r;
}

float Vec3Dot(const Vec3 a, const Vec3 b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 Vec3Cross(const Vec3 a, const Vec3 b) {
  Vec3 r;
  r.x = a.y * b.z - a.z * b.y;
  r.y = a.z * b.x - a.x * b.z;
  r.z = a.x * b.y - a.y * b.x;
  return r;
}

Vec3 Vec3Reflect(const Vec3 d, const Vec3 b) {
  Vec3 n = Vec3Norm(b);
  Vec3 r = Vec3Sub(d, Vec3Scale(n, 2 * Vec3Dot(d, n)));
  return r;
}
