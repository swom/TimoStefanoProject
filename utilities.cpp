#include "utilities.h"

bool are_equal(double lhs, double rhs)
{
  return lhs - rhs < -0.0001 && lhs - rhs > 0.0001;
};

bool are_not_equal(double lhs, double rhs)
{
  return !are_equal(lhs, rhs);
};
