#include "transform.h"

Transform MakeTransform() {
  return (Transform){
      .scale = Vec3One,
      .origin = Vec3Zero,
      .angles = Vec3Zero,
  };
}

Mat4 TransformGetModelMatrix(Transform transform) {
  Mat4 s = Mat4MakeScale(transform.scale);
  Mat4 rx = Mat4MakeRotationX(transform.angles.x);
  Mat4 ry = Mat4MakeRotationY(transform.angles.y);
  Mat4 rz = Mat4MakeRotationZ(transform.angles.z);
  Mat4 m = s;
  m = Mat4Mul(m, rx);
  m = Mat4Mul(m, ry);
  m = Mat4Mul(m, rz);

  Mat4 t = Mat4MakeTranslation(transform.origin);
  m = Mat4Mul(m, t);
  return m;
}
