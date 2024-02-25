#include "xmath.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>

Vec3 Vec3Up() { return (Vec3){0.0f, 1.0f, 0.0f}; }

float Vec3Len(Vec3 v) { return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z); }

float Vec3LenSq(Vec3 v) { return v.x * v.x + v.y * v.y + v.z * v.z; }

Vec3 Vec3Normalize(Vec3 v) {
  float l = Vec3Len(v);
  return (Vec3){v.x / l, v.y / l, v.z / l};
}

Vec3 Vec3Cross(Vec3 a, Vec3 b) {
  return (Vec3){
      a.y * b.z - a.z * b.y,
      a.z * b.x - a.x * b.z,
      a.x * b.y - a.y * b.x,
  };
}

Vec3 Vec3Add(Vec3 a, Vec3 b) { return (Vec3){a.x + b.x, a.y + b.y, a.z + b.z}; }

Vec3 Vec3Sub(Vec3 a, Vec3 b) { return (Vec3){a.x - b.x, a.y - b.y, a.z - b.z}; }

Mat4 Mat4MulMat4(Mat4 l, Mat4 r) {
  // clang-format off
  float xx = l.xx * r.xx + l.xy * r.yx + l.xz * r.zx + l.xw * r.wx;
  float xy = l.xx * r.xy + l.xy * r.yy + l.xz * r.zy + l.xw * r.wy;
  float xz = l.xx * r.xz + l.xy * r.yz + l.xz * r.zz + l.xw * r.wz;
  float xw = l.xx * r.xw + l.xy * r.yw + l.xz * r.zw + l.xw * r.ww;
  float yx = l.yx * r.xx + l.yy * r.yx + l.yz * r.zx + l.yw * r.wx;
  float yy = l.yx * r.xy + l.yy * r.yy + l.yz * r.zy + l.yw * r.wy;
  float yz = l.yx * r.xz + l.yy * r.yz + l.yz * r.zz + l.yw * r.wz;
  float yw = l.yx * r.xw + l.yy * r.yw + l.yz * r.zw + l.yw * r.ww;
  float zx = l.zx * r.xx + l.zy * r.yx + l.zz * r.zx + l.zw * r.wx;
  float zy = l.zx * r.xy + l.zy * r.yy + l.zz * r.zy + l.zw * r.wy;
  float zz = l.zx * r.xz + l.zy * r.yz + l.zz * r.zz + l.zw * r.wz;
  float zw = l.zx * r.xw + l.zy * r.yw + l.zz * r.zw + l.zw * r.ww;
  float wx = l.wx * r.xx + l.wy * r.yx + l.wz * r.zx + l.ww * r.wx;
  float wy = l.wx * r.xy + l.wy * r.yy + l.wz * r.zy + l.ww * r.wy;
  float wz = l.wx * r.xz + l.wy * r.yz + l.wz * r.zz + l.ww * r.wz;
  float ww = l.wx * r.xw + l.wy * r.yw + l.wz * r.zw + l.ww * r.ww;
  return (Mat4) {
    xx, xy, xz, xw,
    yx, yy, yz, yw,
    zx, zy, zz, zw,
    wx, wy, wz, ww,
  };
  // clang-format on
}

Transform TransformIdentity() {
  // clang-format off
  return (Transform){
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
  };
  // clang-format on
}

Transform TransformTranslated(Transform o, Vec3 p) {
  // clang-format off
  return (Transform){
    o.xx, o.xy, o.xz, o.xw + p.x,
    o.yx, o.yy, o.yz, o.yw + p.y,
    o.zx, o.zy, o.zz, o.zw + p.z,
    o.wx, o.wy, o.wz, o.ww,
  };
  // clang-format on
}

Transform TransformLookAt(Transform base, Vec3 target, Vec3 up) {
  Vec3 pos = (Vec3){base.xw, base.yw, base.zw};
  Vec3 dr = Vec3Normalize(Vec3Sub(pos, target));
  Vec3 nr = Vec3Normalize(Vec3Cross(up, dr));
  Vec3 nu = Vec3Cross(dr, nr);
  // clang-format off
  Mat4 left = {
    nr.x, nr.y, nr.z, 0.0f,
    nu.x, nu.y, nu.z, 0.0f,
    dr.x, dr.y, dr.z, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
  };
  Mat4 right = {
    1.0f, 0.0f, 0.0f, -pos.x,
    0.0f, 1.0f, 0.0f, -pos.y,
    0.0f, 0.0f, 1.0f, -pos.z,
    0.0f, 0.0f, 0.0f, 1.0f,
  };
  // clang-format on
  return Mat4MulMat4(left, right);
}

Mat4 MakeProjPerspective(float aspectRatio, float near, float far, float fov) {
  float s = 1.0f / tanf((fov / 2.0f) * DEG2RAD);
  float r = (far - near);
  float a = -(far / r);
  float b = 2.0f * -(far * near / r);
  // clang-format off
  return (Mat4){
       s, 0.0f, 0.0f, 0.0f,
    0.0f,    s, 0.0f, 0.0f,
    0.0f, 0.0f,    a, -1.f,
    0.0f, 0.0f,    b, 0.0f,
  };
  // clang-format on
}
