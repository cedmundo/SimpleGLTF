#pragma once
#include "vec2.h"
#include <math.h>

/**
 * \brief Vector of two dimentions.
 *
 * A defined union to hold references of X and Y components of a 2d vector. Both
 * can be accessible using x,y notation or u,v notation, also alternatively its
 * possible to access them with .xy member wich its a flat array containing both
 * components.
 */
typedef struct {
  float x;
  float y;
} Vec2;

static const Vec2 Vec2Zero = {0.0f, 0.0f};
static const Vec2 Vec2One = {1.0f, 1.0f};

/**
 * \brief Make a Vec2 using its x and y components.
 *
 * Stack vector which components are equal to x and y params.
 * \param x new vector's X component.
 * \param y new vector's Y component.
 * \return newly created vector (stack).
 */
Vec2 Vec2Make(float x, float y);

/**
 * \brief Add two Vec2s.
 *
 * Adds two Vec2 variables, returning the result as a new vector (on stack).
 * \param Vec2 a is the left operand.
 * \param Vec2 b is the right operand.
 * \return a Vec2 with the sum of components of a and b.
 */
Vec2 Vec2Add(const Vec2 a, const Vec2 b);

/**
 * \brief Substract two Vec2s.
 *
 * Substracts two Vec2 variables, returning the result as a new vector (on
 * stack). \param Vec2 a is the left operand. \param Vec2 b is the right
 * operand. \return a Vec2 with the substract of components of a minus b.
 */
Vec2 Vec2Sub(const Vec2 a, const Vec2 b);

/**
 * \brief Scales a vector.
 *
 * Multiplies all components by factor s, returning result as a new vector (on
 * stack). \param Vec2 a is the vector to scale. \param s is the scalar factor.
 * \return the scalation result.
 */
Vec2 Vec2Scale(const Vec2 a, const float s);

/**
 * \brief Adds the multiplication of each vector's components.
 *
 * Inner multiplication is not matematically defined at all, it's more like a
 * handy function to help calculate some other stuff. \param Vec2 a is the left
 * operand. \param Vec2 a is the right operand. \return the sum of the
 * multiplication of each component from both vectors.
 */
float Vec2MulInner(const Vec2 a, const Vec2 b);

/**
 * \brief Square of length of a vector.
 *
 * Calculates the squared length of a vector, use this function instead Vec2Len
 * when comparing or doing operations that doesn't require the exact length.
 * \param Vec2 a the vector.
 * \return it's squared length.
 */
float Vec2SqrLen(const Vec2 a);

/**
 * \brief Calculate a vector's length.
 *
 * Standard vector length calculation, use only when you have to use the exact
 * length otherwise prefer Vec2SqrLen. \param Vec2 a the vector. \return
 * vector's length.
 */
float Vec2Len(const Vec2 a);

/**
 * \brief Normalizes a vector.
 *
 * Standard vector normalization (divide one by the vector's length).
 * \param Vec2 a the vector normalize.
 * \return normalized vector.
 */
Vec2 Vec2Norm(const Vec2 a);

/**
 * \brief Gets the lesser values between two vectors.
 *
 * Creates a new vector (on stack) with the minimum quantities of a and b
 * vectors. \param Vec2 a the left vector. \param Vec2 b the right vector.
 * \return a vector containing two minor values.
 */
Vec2 Vec2Min(const Vec2 a, const Vec2 b);

/**
 * \brief Gets the greater values between two vectors.
 *
 * Creates a new vector (on stack) with the maximum quantities of a and b
 * vectors. \param Vec2 a the left vector. \param Vec2 b the right vector.
 * \return a vector containing two major values.
 */
Vec2 Vec2Max(const Vec2 a, const Vec2 b);
