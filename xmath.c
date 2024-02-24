#include "xmath.h"
#include <assert.h>
#include <stdbool.h>

Vec3 Vec3Up() { return (Vec3){0.0f, 1.0f, 0.0f}; }
Vec3 Vec3Normalize(Vec3 v) { assert(false && "not implemented yet"); }
Vec3 Vec3Cross(Vec3 a, Vec3 b) { assert(false && "not implemented yet"); }
Vec3 Vec3Sub(Vec3 a, Vec3 b) { assert(false && "not implemented yet"); }

Transform TransformIdentity() {
  // clang-format off
  return (Transform){{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
  }};
  // clang-format on
}

Transform TransformTranslated(Transform o, Vec3 v) {
  // clang-format off
  return (Transform){{
    o.xx, o.xy, o.xz, v.x,
    o.yx, o.yy, o.yz, v.y,
    o.zx, o.zy, o.zz, v.z,
    o.wx, o.wy, o.wz, o.ww,
  }};
  // clang-format on
}

Transform TransformLookAt(Transform o, Vec3 t, Vec3 up) {
  // heh:
  // https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/lookat-function/framing-lookat-function.html
  Vec3 m = {o.xw, o.yw, o.zw};
  Vec3 f = Vec3Normalize(Vec3Sub(m, t));
  Vec3 r = Vec3Normalize(Vec3Cross(up, f));
  Vec3 n = Vec3Cross(f, r);
  // clang-format off
  return (Transform){{
     r.x,  r.y,  r.z, o.xw,
     n.x,  n.y,  n.z, o.yw,
     f.x,  f.y,  f.z, o.zw,
     m.x,  m.y,  m.z, o.ww,
  }};
  // clang-format on
}
