#pragma once
#include "scalar.h"
#include "vec3.h"
#include "vec4.h"
#include <assert.h>
#include <math.h>

// clang-format off
/**
 * \brief 4x4 Matrix representation.
 *
 * A flat array containing a 16-size element matrix (4 columns by 4 rows).
 * Data is accessed as column-major so should not be transposed to be used with
 * OpenGL API.
 */
typedef struct {
  float xx, xy, xz, xw;
  float yx, yy, yz, yw;
  float zx, zy, zz, zw;
  float wx, wy, wz, ww;
} Mat4;

static const Mat4 Mat4Zero = {
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
};

static const Mat4 Mat4Identity = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
};
// clang-format on

/**
 * \brief Returns the address of the first element of Mat4 m.
 * \param Mat4 m, matrix to lookup.
 * \return GL-compatible pointer to the buffer
 */
float *Mat4Raw(const Mat4 *m);

/**
 * \brief Gets a matrix row by its index.
 *
 * \param Mat4 m, matrix to lookup in.
 * \param int i, row index, between 0 and 3.
 *  Warning: do not use quantities outside range.
 * \return a Vec4 containing the full row.
 */
Vec4 Mat4Row(const Mat4 m, const unsigned int i);

/**
 * \brief Gets a matrix column by its index.
 *
 * \param Mat4 m, matrix to lookup in.
 * \param int i, column index, between 0 and 3. Warning: do not use quantities
 * outside range. \return a Vec4 containing the full column.
 */
Vec4 Mat4Col(const Mat4 m, const unsigned int i);

/**
 * \brief Transposes a matrix.
 *
 * \param Mat4 m matrix to be transposed (not modified).
 * \return transposed matrix.
 */
Mat4 Mat4Transpose(const Mat4 m);

/**
 * \brief Adds two matrices.
 * \param Mat4 a left operand.
 * \param Mat4 b right operand.
 * \return a new stack matrix with a+b components result.
 */
Mat4 Mat4Add(const Mat4 a, const Mat4 b);

/**
 * \brief Substracts two matrices.
 * \param Mat4 a left operand.
 * \param Mat4 b right operand.
 * \return a new stack matrix with a-b components result.
 */
Mat4 Mat4Sub(const Mat4 a, const Mat4 b);

/**
 * \brief Scales a matrix by a scalar factor.
 * \param Mat4 a matrix to scale.
 * \param float s scalar factor.
 * \return new stack scaled matrix.
 */
Mat4 Mat4Scale(const Mat4 a, float s);

/**
 * \brief Multiplies two matrices.
 * \param Mat4 a left operand.
 * \param Mat4 b right operand.
 * \return a result of multiplication between axb.
 */
Mat4 Mat4Mul(const Mat4 a, const Mat4 b);

/**
 * \brief Multiplies a vector with a matrix.
 */
Vec4 Mat4MulVec4(const Mat4 a, const Vec4 b);

/**
 * \brief Makes a new model scale matrix.
 *
 * \param Vec3 u the scale for X, Y and Z coordinates.
 * \return scale matrix.
 */
Mat4 Mat4MakeScale(Vec3 u);

/**
 * \brief Makes a new model translation matrix.
 *
 * \param Vec3 u the scale for X, Y and Z coordinates.
 * \return scale matrix.
 */
Mat4 Mat4MakeTranslation(Vec3 u);

/**
 * \brief Makes a new model rotation matrix.
 *
 * \param Vec3 u the rotation axis to apply angle.
 * \param float angle the angle to rotate.
 * \return rotation matrix.
 */
Mat4 Mat4MakeRotation(Vec3 u, float angle);

/**
 * \brief Makes a new model rotation matrix only on X axis (euler).
 *
 * \param float angle in radians.
 * \return rotation matrix on X axis.
 */
Mat4 Mat4MakeRotationX(float angle);

/**
 * \brief Makes a new model rotation matrix only on Y axis (euler).
 *
 * \param float angle in radians.
 * \return rotation matrix on Y axis.
 */
Mat4 Mat4MakeRotationY(float angle);

/**
 * \brief Makes a new model rotation matrix only on Z axis (euler).
 *
 * \param float angle in radians.
 * \return rotation matrix on Z axis.
 */
Mat4 Mat4MakeRotationZ(float angle);

/**
 * \brief Makes a new ortho matrix.
 *
 * Generates a new projection matrix usen left, right, bottom, top, near and far
 * parameters.
 *
 * \param float l left
 * \param float r right
 * \param float b bottom
 * \param float t top
 * \param float n near
 * \param float f far
 * \return a othographic projection matrix.
 */
Mat4 Mat4MakeOrtho(float l, float r, float b, float t, float n, float f);

/**
 * \brief Makes a new perspective matrix.
 *
 * Generates a new projection matrix usen fov, aspect, near and far parameters.
 *
 * \param float yfov Field of view from Y axis
 * \param float aspect ratio
 * \param float n near
 * \param float f far
 * \return a prerspective projection matrix.
 */
Mat4 Mat4MakePerspective(float yfov, float aspect, float n, float f);

/**
 * \brief Makes a new "looking at" matrix.
 *
 * \param float position of object
 * \param float target to look at
 * \param float up
 */
Mat4 Mat4LookAt(Vec3 position, Vec3 target, Vec3 up);
