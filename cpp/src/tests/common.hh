#include <gmock/gmock.h>
#include "common/common.hh"

namespace rracer {
  void ASSERT_VECTOR_EQ(const Vector& a, const Vector& b);
  void ASSERT_MATRIX_EQ(const AffineTransform& a, const AffineTransform& b);
}; // end ns::rracer
