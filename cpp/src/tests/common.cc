#include "tests/common.hh"

#define NEAR_EPSILON 0.000001

namespace rracer {
  void ASSERT_VECTOR_EQ(const Vector& a, const Vector& b)
  {
    ASSERT_NEAR(a[0], b[0], NEAR_EPSILON);
    ASSERT_NEAR(a[1], b[1], NEAR_EPSILON);
  }


  void ASSERT_MATRIX_EQ(const AffineTransform& a, const AffineTransform& b) {
    for(int i = 0; i < 3; i++) {
      for(int j = 0; j < 3; j++) {
	ASSERT_NEAR(a(i, j), b(i, j), NEAR_EPSILON);
      }
    }
  }

}; // end ns::rracer
