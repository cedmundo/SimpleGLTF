#pragma once
#include "vec3.h"

#include "vec2.h"
#include <math.h>

/**
 * \brief Vector of three dimentions.
 *
 * A defined union to hold references of X, Y and Z components  of a vector with
 * three components, accessible using (xyz), (rgb) and (yaw,pitch,roll)
 * notations.
 */
typedef struct {
  float x;
  float y;
  float z;
} Vec3;

static const Vec3 Vec3Zero = {0.0f, 0.0f, 0.0f};
static const Vec3 Vec3Up = {0.0f, 1.0f, 0.0f};
static const Vec3 Vec3Down = {0.0f, -1.0f, 0.0f};
static const Vec3 Vec3Forward = {0.0f, 0.0f, -1.0f};
static const Vec3 Vec3Backward = {0.0f, 0.0f, 1.0f};
static const Vec3 Vec3Left = {-1.0f, 0.0f, 0.0f};
static const Vec3 Vec3Right = {1.0f, 0.0f, 0.0f};
static const Vec3 Vec3One = {1.0f, 1.0f, 1.0f};

/**
 * \brief Makes a Vec3 using x, y and z components.
 *
 * Make a vector which components are equal to x, y and z params.
 * \param x new vector's X component.
 * \param y new vector's Y component.
 * \param z new vector's Z component.
 * \return newly created vector (stack).
 */
Vec3 Vec3Make(float x, float y, float z);

/**
 * \brief Makes a Vec3 using a Vec3 and z component.
 *
 * Make a vector which components are equal to x, y (from Vec2) and z params.
 * \param Vec2 v origin of X and Y components.
 * \param z new vector's Z component.
 * \return newly created vector (stack).
 */
Vec3 Vec3MakeVz(Vec2 v, float z);

/**
 * \brief Add two Vec3s.
 *
 * Adds two Vec3 variables, returning the result as a new vector (on stack).
 * \param Vec3 a is the left operand.
 * \param Vec3 b is the right operand.
 * \return a Vec3 with the sum of components of a and b.
 */
Vec3 Vec3Add(const Vec3 a, const Vec3 b);

/**
 * \brief Substract two Vec3s.
 *
 * Substracts two Vec3 variables, returning the result as a new vector (on
 * stack). \param Vec3 a is the left operand. \param Vec3 b is the right
 * operand. \return a Vec3 with the substract of components of a minus b.
 */
Vec3 Vec3Sub(const Vec3 a, const Vec3 b);

/**
 * \brief Scales a Vec3.
 *
 * Multiplies all components by factor s, returning result as a new vector (on
 * stack). \param Vec3 a is the vector to scale. \param s is the scalar factor.
 * \return the scalation result.
 */
Vec3 Vec3Scale(const Vec3 a, const float s);

/**
 * \brief Adds the multiplication of each vector's components.
 *
 * Inner multiplication is not matematically defined at all, it's more like a
 * handy function to help calculate some other stuff. \param Vec3 a is the left
 * operand. \param Vec3 a is the right operand. \return the sum of the
 * multiplication of each component from both vectors.
 */
float Vec3MulInner(const Vec3 a, const Vec3 b);

/**
 * \brief Square of length of a vector.
 *
 * Calculates the squared length of a vector, use this function instead Vec3Len
 * when comparing or doing operations that doesn't require the exact length.
 * \param Vec3 a the vector.
 * \return it's squared length.
 */
float Vec3SqrLen(const Vec3 a);

/**
 * \brief Calculate a vector's length.
 *
 * Standard vector length calculation, use only when you have to use the exact
 * length otherwise prefer Vec3SqrLen. \param Vec3 a the vector. \return
 * vector's length.
 */
float Vec3Len(const Vec3 a);

/**
 * \brief Normalizes a vector.
 *
 * Standard vector normalization (divide one by the vector's length).
 * \param Vec3 a the vector normalize.
 * \return normalized vector.
 */
Vec3 Vec3Norm(const Vec3 a);

/**
 * \brief Gets the lesser values between two vectors.
 *
 * Creates a new vector (on stack) with the minimum quantities of a and b
 * vectors. \param Vec3 a the left vector. \param Vec3 b the right vector.
 * \return a vector containing two minor values.
 */
Vec3 Vec3Min(const Vec3 a, const Vec3 b);

/**
 * \brief Gets the greater values between two vectors.
 *
 * Creates a new vector (on stack) with the maximum quantities of a and b
 * vectors. \param Vec3 a the left vector. \param Vec3 b the right vector.
 * \return a vector containing two major values.
 */
Vec3 Vec3Max(const Vec3 a, const Vec3 b);

/**
 * \brief Dot product between two Vec3s.
 *
 * \param Vec3 a the left vector.
 * \param Vec3 b the right vector.
 * \return result of dot product.
 */
float Vec3Dot(const Vec3 a, const Vec3 b);

/**
 * \brief Cross product between two Vec3s.
 *
 * \param Vec3 a the left vector.
 * \param Vec3 b the right vector.
 * \return result of cross product.
 */
Vec3 Vec3Cross(const Vec3 a, const Vec3 b);

/**
 * \brief Calcualtes a reflection vector from a across b.
 *
 * \param Vec3 a the left vector.
 * \param Vec3 b the right vector.
 * \return reflection vector.
 */
Vec3 Vec3Reflect(const Vec3 d, const Vec3 b);
