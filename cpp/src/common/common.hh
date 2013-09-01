#ifndef COMMON_HH
#define COMMON_HH

#include <vector>
#include <cassert>
#include <math.h>

#include <Eigen/Eigen>

#define DEG2RAD(x) ((x / 180.0) * M_PI)
#define SIGN(x) ((x < 0 ? -1 : 1))


using namespace std;

namespace rracer {

  typedef Eigen::Affine2f AffineTransform;
  typedef Eigen::Vector2f Vector;
  typedef float Real;

  class Rect {

  public:
    Vector origin;
    Vector size;

    Rect();
    Rect(const Vector& origin, const Vector& size);
    Rect(Real x, Real y, Real width, Real height);
    static Rect from_corners(const Vector& bottom_left, const Vector& top_right);
    static Rect from_points(const vector<Vector>&);
    virtual ~Rect();

    Real left() const;
    Real right() const;
    Real top() const;
    Real bottom() const;

    bool empty() const;

    bool operator==(const Rect&) const;
    Rect operator|(const Rect&) const;

  };

  AffineTransform rotation(int degree);

  /**
   * Used to classify points
   * based on a central point
   *
   * Q1    A1    Q0
   *       |
   *       |
   * A2----+-----A0
   *       |
   *       |
   * Q2    A3    Q3
   */
  enum EQuarterClass {
    EQ, // when the points are exactly the same
    A0,
    Q0,
    A1,
    Q1,
    A2,
    Q2,
    A3,
    Q3
  };

  /**
   * classify a point
   * relative to the center into
   * the above enum
   */
  EQuarterClass classify_point(const Vector& cp, const Vector& p);

  /**
   * For a given center and radius, produce a point lying on the
   * also given axis.
   */
  Vector axis_point(const Vector& cp, const Real& radius, const EQuarterClass& axis);
}

#endif
