#include "mat4.h"

float *Mat4Raw(const Mat4 *m) { return (float *)&m->xx; }

Vec4 Mat4Row(const Mat4 m, const unsigned int i) {
  assert(i <= 4);

  Vec4 r;
  float *raw = Mat4Raw(&m);
  r.x = raw[(i * 4)]; // +0
  r.y = raw[(i * 4) + 1];
  r.z = raw[(i * 4) + 2];
  r.w = raw[(i * 4) + 3];
  return r;
}

Vec4 Mat4Col(const Mat4 m, const unsigned int i) {
  assert(i <= 3);

  Vec4 r;
  float *raw = Mat4Raw(&m);
  r.x = raw[i]; // +0
  r.y = raw[i + 4];
  r.z = raw[i + 8];
  r.w = raw[i + 12];
  return r;
}

Mat4 Mat4Transpose(const Mat4 m) {
  Mat4 r;

  r.xx = m.xx;
  r.xy = m.yx;
  r.xz = m.zx;
  r.xw = m.wx;
  r.yx = m.xy;
  r.yy = m.yy;
  r.yz = m.zy;
  r.yw = m.wy;
  r.zx = m.xz;
  r.zy = m.yz;
  r.zz = m.zz;
  r.zw = m.wz;
  r.wx = m.xw;
  r.wy = m.yw;
  r.wz = m.zw;
  r.ww = m.ww;

  return r;
}

Mat4 Mat4Add(const Mat4 a, const Mat4 b) {
  Mat4 r;
  r.xx = a.xx + b.xx;
  r.xy = a.xy + b.xy;
  r.xz = a.xz + b.xz;
  r.xw = a.xw + b.xw;
  r.yx = a.yx + b.yx;
  r.yy = a.yy + b.yy;
  r.yz = a.yz + b.yz;
  r.yw = a.yw + b.yw;
  r.zx = a.zx + b.zx;
  r.zy = a.zy + b.zy;
  r.zz = a.zz + b.zz;
  r.zw = a.zw + b.zw;
  r.wx = a.wx + b.wx;
  r.wy = a.wy + b.wy;
  r.wz = a.wz + b.wz;
  r.ww = a.ww + b.ww;
  return r;
}

Mat4 Mat4Sub(const Mat4 a, const Mat4 b) {
  Mat4 r;
  r.xx = a.xx - b.xx;
  r.xy = a.xy - b.xy;
  r.xz = a.xz - b.xz;
  r.xw = a.xw - b.xw;
  r.yx = a.yx - b.yx;
  r.yy = a.yy - b.yy;
  r.yz = a.yz - b.yz;
  r.yw = a.yw - b.yw;
  r.zx = a.zx - b.zx;
  r.zy = a.zy - b.zy;
  r.zz = a.zz - b.zz;
  r.zw = a.zw - b.zw;
  r.wx = a.wx - b.wx;
  r.wy = a.wy - b.wy;
  r.wz = a.wz - b.wz;
  r.ww = a.ww - b.ww;
  return r;
}

Mat4 Mat4Scale(const Mat4 a, float s) {
  Mat4 r;
  r.xx = a.xx * s;
  r.xy = a.xy * s;
  r.xz = a.xz * s;
  r.xw = a.xw * s;
  r.yx = a.yx * s;
  r.yy = a.yy * s;
  r.yz = a.yz * s;
  r.yw = a.yw * s;
  r.zx = a.zx * s;
  r.zy = a.zy * s;
  r.zz = a.zz * s;
  r.zw = a.zw * s;
  r.wx = a.wx * s;
  r.wy = a.wy * s;
  r.wz = a.wz * s;
  r.ww = a.ww * s;
  return r;
}

Mat4 Mat4Mul(const Mat4 a, const Mat4 b) {
  Mat4 r = Mat4Zero;

  r.xx = a.xx * b.xx + a.xy * b.yx + a.xz * b.zx + a.xw * b.wx;
  r.xy = a.xx * b.xy + a.xy * b.yy + a.xz * b.zy + a.xw * b.wy;
  r.xz = a.xx * b.xz + a.xy * b.yz + a.xz * b.zz + a.xw * b.wz;
  r.xw = a.xx * b.xw + a.xy * b.yw + a.xz * b.zw + a.xw * b.ww;

  r.yx = a.yx * b.xx + a.yy * b.yx + a.yz * b.zx + a.yw * b.wx;
  r.yy = a.yx * b.xy + a.yy * b.yy + a.yz * b.zy + a.yw * b.wy;
  r.yz = a.yx * b.xz + a.yy * b.yz + a.yz * b.zz + a.yw * b.wz;
  r.yw = a.yx * b.xw + a.yy * b.yw + a.yz * b.zw + a.yw * b.ww;

  r.zx = a.zx * b.xx + a.zy * b.yx + a.zz * b.zx + a.zw * b.wx;
  r.zy = a.zx * b.xy + a.zy * b.yy + a.zz * b.zy + a.zw * b.wy;
  r.zz = a.zx * b.xz + a.zy * b.yz + a.zz * b.zz + a.zw * b.wz;
  r.zw = a.zx * b.xw + a.zy * b.yw + a.zz * b.zw + a.zw * b.ww;

  r.wx = a.wx * b.xx + a.wy * b.yx + a.wz * b.zx + a.ww * b.wx;
  r.wy = a.wx * b.xy + a.wy * b.yy + a.wz * b.zy + a.ww * b.wy;
  r.wz = a.wx * b.xz + a.wy * b.yz + a.wz * b.zz + a.ww * b.wz;
  r.ww = a.wx * b.xw + a.wy * b.yw + a.wz * b.zw + a.ww * b.ww;

  return r;
}

Vec4 Mat4MulVec4(const Mat4 a, const Vec4 b) {
  Vec4 r;

  r.x = a.xx * b.x + a.xy * b.y + a.xz * b.z + a.xw * b.w;
  r.y = a.yx * b.x + a.yy * b.y + a.yz * b.z + a.yw * b.w;
  r.z = a.zx * b.x + a.zy * b.y + a.zz * b.z + a.zw * b.w;
  r.w = a.wx * b.x + a.wy * b.y + a.wz * b.z + a.ww * b.w;

  return r;
}

Mat4 Mat4MakeScale(Vec3 u) {
  Mat4 r = Mat4Identity;
  r.xx = u.x;
  r.yy = u.y;
  r.zz = u.z;
  return r;
}

Mat4 Mat4MakeTranslation(Vec3 u) {
  Mat4 r = Mat4Identity;
  r.wx = u.x;
  r.wy = u.y;
  r.wz = u.z;
  return r;
}

Mat4 Mat4MakeRotation(Vec3 u, float angle) {
  Mat4 r = Mat4Identity;
  u = Vec3Norm(u);

  float a_cos = cosf(angle);
  float a_sin = sinf(angle);
  float a_icos = (1 - cosf(angle));

  r.xx = a_cos + u.x * u.x * a_icos;
  r.xy = u.x * u.y * a_icos + u.z * a_sin;
  r.xz = u.x * u.z * a_icos + u.y * a_sin;

  r.yx = u.y * u.x * a_icos - u.z * a_sin;
  r.yy = a_cos + u.y * u.y * a_icos;
  r.yz = u.y * u.z * a_icos + u.x * a_sin;

  r.zx = u.z * u.x * a_icos - u.y * a_sin;
  r.zy = u.z * u.y * a_icos - u.x * a_sin;
  r.zz = a_cos + u.z * u.z * a_icos;

  return r;
}

Mat4 Mat4MakeRotationX(float angle) {
  Mat4 r = Mat4Identity;
  float c = cosf(angle);
  float s = sinf(angle);

  r.yy = c;
  r.yz = s;

  r.zy = -s;
  r.zz = c;
  return r;
}

Mat4 Mat4MakeRotationY(float angle) {
  Mat4 r = Mat4Identity;
  float c = cosf(angle);
  float s = sinf(angle);

  r.xx = c;
  r.xz = -s;

  r.zx = s;
  r.zz = c;
  return r;
}

Mat4 Mat4MakeRotationZ(float angle) {
  Mat4 r = Mat4Identity;
  float c = cosf(angle);
  float s = sinf(angle);

  r.xx = c;
  r.xy = s;

  r.yx = -s;
  r.yy = c;
  return r;
}

Mat4 Mat4MakeOrtho(float l, float r, float b, float t, float n, float f) {
  Mat4 m = Mat4Zero;

  m.xx = 2.0f / (r - l);
  m.yy = 2.0f / (t - b);
  m.zz = -2.0f / (f - n);

  m.wx = -(r + l) / (r - l);
  m.wy = -(t + b) / (t - b);
  m.wz = -(f + n) / (f - n);
  m.ww = 1.0f;

  return m;
}

Mat4 Mat4MakePerspective(float yfov, float aspect, float n, float f) {
  Mat4 m = Mat4Zero;
  float const a = 1.0f / tanf(yfov * DEG2RAD / 2.0f);

  m.xx = a / aspect;
  m.xy = 0.0f;
  m.xz = 0.0f;
  m.xw = 0.0f;

  m.yx = 0.0f;
  m.yy = a;
  m.yz = 0.0f;
  m.yw = 0.0f;

  m.zx = 0.0f;
  m.zy = 0.0f;
  m.zz = -((f + n) / (f - n));
  m.zw = -1.0f;

  m.wx = 0.0f;
  m.wy = 0.0f;
  m.wz = -((2.0f * f * n) / (f - n));
  m.ww = 0.0f;
  return m;
}

Mat4 Mat4LookAt(Vec3 position, Vec3 target, Vec3 up) {
  Mat4 m = Mat4Identity;
  Vec3 f = Vec3Norm(Vec3Sub(position, target));
  Vec3 u = Vec3Norm(up);
  Vec3 s = Vec3Norm(Vec3Cross(f, u));
  u = Vec3Cross(s, f);

  m.xx = s.x;
  m.yx = s.y;
  m.zx = s.z;
  m.xy = u.x;
  m.yy = u.y;
  m.zy = u.z;
  m.xz = -f.x;
  m.yz = -f.y;
  m.zz = -f.z;
  m.wx = -Vec3Dot(s, target);
  m.wy = -Vec3Dot(u, target);
  m.wz = Vec3Dot(f, target);
  return m;
}
