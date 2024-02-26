#include "scalar.h"

bool FAproxEq(float a, float b) {
  return a > b ? ((a - b) <= EPSILON) : ((b - a) <= EPSILON);
}
