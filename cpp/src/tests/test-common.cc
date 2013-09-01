#include <gmock/gmock.h>
#include <boost/assign/list_of.hpp>

#include "common/common.hh"

using namespace boost::assign;

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

  Rect d = Rect::from_center_and_size(Vector(10, 10), Vector(20, 20));
  ASSERT_EQ(Rect(0, 0, 20, 20), d);
  ASSERT_EQ(Vector(10, 10), d.center());
  Rect e = d * 2.0;

  ASSERT_EQ(d.center(), e.center());
  ASSERT_FLOAT_EQ(d.size[0] * 2.0, (d * 2.0).size[0]);
  ASSERT_FLOAT_EQ(d.size[1] * 2.0, (d * 2.0).size[1]);

  ASSERT_TRUE(a.contains(Vector(10, 20)));

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


TEST(CommonTests, TestSpanningQuadrants) {
  ASSERT_EQ(list_of(A1), spanning_quadrants(Q0, Q1));
  ASSERT_EQ(list_of(A2), spanning_quadrants(Q1, Q2));
  ASSERT_EQ(list_of(A3), spanning_quadrants(Q2, Q3));
  ASSERT_EQ(list_of(A0), spanning_quadrants(Q3, Q0));

  ASSERT_EQ(list_of(A1)(A2), spanning_quadrants(Q0, Q2));
  ASSERT_EQ(list_of(A2)(A3), spanning_quadrants(Q1, Q3));
  ASSERT_EQ(list_of(A3)(A0), spanning_quadrants(Q2, Q0));
  ASSERT_EQ(list_of(A0)(A1), spanning_quadrants(Q3, Q1));

  ASSERT_EQ(list_of(A1)(A2)(A3), spanning_quadrants(Q0, Q3));
  ASSERT_EQ(list_of(A2)(A3)(A0), spanning_quadrants(Q1, Q0));
  ASSERT_EQ(list_of(A3)(A0)(A1), spanning_quadrants(Q2, Q1));
  ASSERT_EQ(list_of(A0)(A1)(A2), spanning_quadrants(Q3, Q2));

  ASSERT_EQ(list_of(A1)(A2)(A3)(A0), spanning_quadrants(Q0, Q0));
  ASSERT_EQ(list_of(A2)(A3)(A0)(A1), spanning_quadrants(Q1, Q1));
  ASSERT_EQ(list_of(A3)(A0)(A1)(A2), spanning_quadrants(Q2, Q2));
  ASSERT_EQ(list_of(A0)(A1)(A2)(A3), spanning_quadrants(Q3, Q3));

  ASSERT_EQ(list_of(A1), spanning_quadrants(A0, A2));
  ASSERT_EQ(list_of(A1)(A2)(A3), spanning_quadrants(A0, A0));
}


TEST(CommonTests, TestRectFitting) {
  Rect a = Rect::from_center_and_size(Vector(0, 0), Vector(10, 10));
  Rect b = Rect::from_center_and_size(Vector(0, 0), Vector(20, 20));
  AffineTransform t = b.fit(a);
  ASSERT_FLOAT_EQ(2.0, t(0, 0));
  ASSERT_FLOAT_EQ(2.0, t(1, 1));

  Rect c = Rect::from_center_and_size(Vector(0, 0), Vector(10, 10));
  Rect d = Rect::from_center_and_size(Vector(10, 20), Vector(10, 10));
  AffineTransform t2 = d.fit(c);
  ASSERT_FLOAT_EQ(10, t2(0, 2));
  ASSERT_FLOAT_EQ(20, t2(1, 2));

}
