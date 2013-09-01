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


TEST(CommonTests, TestRectOperations) {
  Rect a(10, 20, 30, 40);
  Rect b(Vector(10, 20), Vector(30, 40));
  ASSERT_EQ(a, b);
  ASSERT_EQ(a, a | b);
  ASSERT_EQ(10, a.left());
  ASSERT_EQ(60, a.top());
  ASSERT_EQ(40, a.right());
  ASSERT_EQ(20, a.bottom());

  Rect c;
  ASSERT_TRUE(c.empty());
  ASSERT_EQ(a, a | c);
  ASSERT_EQ(a, c | a);
}


TEST(CommonTests, TestQuarterClassification) {
  Real x = 0;
  Real y = 0;
  Vector cp(x, y);
  ASSERT_EQ(rracer::EQ, classify_point(cp, cp));
  ASSERT_EQ(rracer::A0, classify_point(cp, Vector(x + 10, y)));
  ASSERT_EQ(rracer::Q0, classify_point(cp, Vector(x + 10, y + 10)));
  ASSERT_EQ(rracer::A1, classify_point(cp, Vector(x, y + 10)));
  ASSERT_EQ(rracer::Q1, classify_point(cp, Vector(x - 10, y + 10)));
  ASSERT_EQ(rracer::A2, classify_point(cp, Vector(x - 10, y)));
  ASSERT_EQ(rracer::Q2, classify_point(cp, Vector(x - 10, y - 10)));
  ASSERT_EQ(rracer::A3, classify_point(cp, Vector(x, y - 10)));
  ASSERT_EQ(rracer::Q3, classify_point(cp, Vector(x + 10, y - 10)));
}


TEST(CommonTests, TestAxisPointGeneration) {
  Real x = 0;
  Real y = 0;
  Real radius = 10.0;
  Vector cp(x, y);
  ASSERT_EQ(Vector(x + radius, y), axis_point(cp, radius, A0));
  ASSERT_EQ(Vector(x, y + radius), axis_point(cp, radius, A1));
  ASSERT_EQ(Vector(x - radius, y), axis_point(cp, radius, A2));
  ASSERT_EQ(Vector(x, y - radius), axis_point(cp, radius, A3));
}
