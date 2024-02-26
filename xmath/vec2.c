#include "vec2.h"

Vec2 Vec2Make(float x, float y) {
  Vec2 r;
  r.x = x;
  r.y = y;
  return r;
}

Vec2 Vec2Add(const Vec2 a, const Vec2 b) {
  Vec2 r;
  r.x = a.x + b.x;
  r.y = a.y + b.y;
  return r;
}

Vec2 Vec2Sub(const Vec2 a, const Vec2 b) {
  Vec2 r;
  r.x = a.x - b.x;
  r.y = a.y - b.y;
  return r;
}

Vec2 Vec2Scale(const Vec2 a, const float s) {
  Vec2 r;
  r.x = a.x * s;
  r.y = a.y * s;
  return r;
}

float Vec2MulInner(const Vec2 a, const Vec2 b) {
  float r = 0.0f;
  r += a.x * b.x;
  r += a.y * b.y;
  return r;
}

float Vec2SqrLen(const Vec2 a) { return Vec2MulInner(a, a); }

float Vec2Len(const Vec2 a) { return sqrtf(Vec2SqrLen(a)); }

Vec2 Vec2Norm(const Vec2 a) {
  float k = 1.0f / Vec2Len(a);
  return Vec2Scale(a, k);
}

Vec2 Vec2Min(const Vec2 a, const Vec2 b) {
  Vec2 r;
  r.x = a.x < b.x ? a.x : b.x;
  r.y = a.y < b.y ? a.y : b.y;
  return r;
}

Vec2 Vec2Max(const Vec2 a, const Vec2 b) {
  Vec2 r;
  r.x = a.x > b.x ? a.x : b.x;
  r.y = a.y > b.y ? a.y : b.y;
  return r;
}
