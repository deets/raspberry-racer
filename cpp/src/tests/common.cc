#include "tests/common.hh"

namespace rracer {
  void ASSERT_VECTOR_EQ(const Vector& a, const Vector& b)
  {
    ASSERT_NEAR(a[0], b[0], 0.000001);
    ASSERT_NEAR(a[1], b[1], 0.000001);
  }
}; // end ns::rracer
