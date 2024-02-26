#include "vec4.h"

Vec4 Vec4Make(float x, float y, float z, float w) {
  Vec4 r;
  r.x = x;
  r.y = y;
  r.z = z;
  r.w = w;
  return r;
}

Vec4 Vec4MakeVw(Vec3 v, float w) {
  Vec4 r;
  r.x = v.x;
  r.y = v.y;
  r.z = v.z;
  r.w = w;
  return r;
}

Vec4 Vec4MakeVzw(Vec2 v, float z, float w) {
  Vec4 r;
  r.x = v.x;
  r.y = v.y;
  r.z = z;
  r.w = w;
  return r;
}

Vec4 Vec4Add(const Vec4 a, const Vec4 b) {
  Vec4 r;
  r.x = a.x + b.x;
  r.y = a.y + b.y;
  r.z = a.z + b.z;
  r.w = a.w + b.w;
  return r;
}

Vec4 Vec4Sub(const Vec4 a, const Vec4 b) {
  Vec4 r;
  r.x = a.x - b.x;
  r.y = a.y - b.y;
  r.z = a.z - b.z;
  r.w = a.w - b.w;
  return r;
}

Vec4 Vec4Scale(const Vec4 a, const float s) {
  Vec4 r;
  r.x = a.x * s;
  r.y = a.y * s;
  r.z = a.z * s;
  r.w = a.w * s;
  return r;
}

float Vec4MulInner(const Vec4 a, const Vec4 b) {
  float r = 0.0f;
  r += a.x * b.x;
  r += a.y * b.y;
  r += a.z * b.z;
  r += a.w * b.w;
  return r;
}

float Vec4SqrLen(const Vec4 a) { return Vec4MulInner(a, a); }

float Vec4Len(const Vec4 a) { return sqrtf(Vec4SqrLen(a)); }

Vec4 Vec4Norm(const Vec4 a) {
  float k = 1.0f / Vec4Len(a);
  return Vec4Scale(a, k);
}

Vec4 Vec4Min(const Vec4 a, const Vec4 b) {
  Vec4 r;
  r.x = a.x < b.x ? a.x : b.x;
  r.y = a.y < b.y ? a.y : b.y;
  r.z = a.z < b.z ? a.z : b.z;
  r.w = a.w < b.w ? a.w : b.w;
  return r;
}

Vec4 Vec4Max(const Vec4 a, const Vec4 b) {
  Vec4 r;
  r.x = a.x > b.x ? a.x : b.x;
  r.y = a.y > b.y ? a.y : b.y;
  r.z = a.z > b.z ? a.z : b.z;
  r.w = a.w > b.w ? a.w : b.w;
  return r;
}
