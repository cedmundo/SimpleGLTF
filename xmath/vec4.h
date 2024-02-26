#include "vec2.h"
#include "vec3.h"
#include <math.h>

/**
 * \brief Vector of four dimentions.
 *
 * A defined union to hold references of X, Y, Z and W components  of a vector
 * with three components, accessible using (xyzw) and (rgba) notations.
 */
typedef struct {
  float x;
  float y;
  float z;
  float w;
} Vec4;

static const Vec4 Vec4Zero = {0.0f, 0.0f, 0.0f, 0.0f};
static const Vec4 Vec4One = {1.0f, 1.0f, 1.0f, 1.0f};

/**
 * \brief Makes a Vec4 using x, y and z components.
 *
 * Make a vector which components are equal to x, y, z and w params.
 * \param x new vector's X component.
 * \param y new vector's Y component.
 * \param z new vector's Z component.
 * \param w new vector's W component.
 * \return newly created vector (stack).
 */
Vec4 Vec4Make(float x, float y, float z, float w);

/**
 * \brief Makes a Vec4 using a Vec3 and w component.
 *
 * Make a vector which components are equal to x, y, z (from Vec3) and w params.
 * \param Vec3 v origin of X, Y and Z components.
 * \param w new vector's W component.
 * \return newly created vector (stack).
 */
Vec4 Vec4MakeVw(Vec3 v, float w);

/**
 * \brief Makes a Vec4 using a Vec2, z and w components.
 *
 * Make a vector which components are equal to x, y (from Vec2), z and w params.
 * \param vec2 v origin of X and Y components.
 * \param z new vector's Z component.
 * \param w new vector's W component.
 * \return newly created vector (stack).
 */
Vec4 Vec4MakeVzw(Vec2 v, float z, float w);

/**
 * \brief Add two Vec4s.
 *
 * Adds two Vec4 variables, returning the result as a new vector (on stack).
 * \param Vec4 a is the left operand.
 * \param Vec4 b is the right operand.
 * \return a Vec4 with the sum of components of a and b.
 */
Vec4 Vec4Add(const Vec4 a, const Vec4 b);

/**
 * \brief Substract two Vec4s.
 *
 * Substracts two Vec4 variables, returning the result as a new vector (on
 * stack).
 * \param Vec4 a is the left operand.
 * \param Vec4 b is the right operand.
 * \return a Vec4 with the substract of components of a minus b.
 */
Vec4 Vec4Sub(const Vec4 a, const Vec4 b);

/**
 * \brief Scales a Vec4.
 *
 * Multiplies all components by factor s, returning result as a new vector (on
 * stack).
 * \param Vec4 a is the vector to scale.
 * \param s is the scalar factor.
 * \return the scalation result.
 */
Vec4 Vec4Scale(const Vec4 a, const float s);

/**
 * \brief Adds the multiplication of each vector's components.
 *
 * Inner multiplication is not matematically defined at all, it's more like a
 * handy function to help calculate some other stuff.
 * \param Vec4 a is the left operand.
 * \param Vec4 a is the right operand.
 * \return the sum of the multiplication of each component from both vectors.
 */
float Vec4MulInner(const Vec4 a, const Vec4 b);

/**
 * \brief Square of length of a vector.
 *
 * Calculates the squared length of a vector, use this function instead Vec4Len
 * when comparing or doing operations that doesn't require the exact length.
 * \param Vec4 a the vector.
 * \return it's squared length.
 */
float Vec4SqrLen(const Vec4 a);

/**
 * \brief Calculate a vector's length.
 *
 * Standard vector length calculation, use only when you have to use the exact
 * length otherwise prefer Vec4SqrLen.
 * \param Vec4 a the vector. \return
 * vector's length.
 */
float Vec4Len(const Vec4 a);

/**
 * \brief Normalizes a vector.
 *
 * Standard vector normalization (divide one by the vector's length).
 * \param Vec4 a the vector normalize.
 * \return normalized vector.
 */
Vec4 Vec4Norm(const Vec4 a);

/**
 * \brief Gets the lesser values between two vectors.
 *
 * Creates a new vector (on stack) with the minimum quantities of a and b
 * vectors.
 * \param vec2 a the left vector.
 * \param vec2 b the right vector.
 * \return a vector containing two minor values.
 */
Vec4 Vec4Min(const Vec4 a, const Vec4 b);

/**
 * \brief Gets the greater values between two vectors.
 *
 * Creates a new vector (on stack) with the maximum quantities of a and b
 * vectors.
 * \param Vec4 a the left vector.
 * \param Vec4 b the right vector.
 * \return a vector containing two major values.
 */
Vec4 Vec4Max(const Vec4 a, const Vec4 b);
