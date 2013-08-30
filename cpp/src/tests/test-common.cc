#include <gmock/gmock.h>

#include "common/common.hh"

using namespace rracer;

TEST(CommonTests, TestSomeMath) {
  Vector v;
  v << 1, 0;
  AffineTransform t = rotation(90);
  Vector u = t * v;
  ASSERT_NEAR(0, u[0], 0.0000001);
  ASSERT_FLOAT_EQ(1, u[1]);
  ASSERT_EQ(-1, SIGN(-10));
  ASSERT_EQ(1, SIGN(10));
  ASSERT_EQ(1, SIGN(0));
}
