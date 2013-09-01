#ifndef COMMON_HH
#define COMMON_HH

#include <cassert>
#include <math.h>

#include <Eigen/Eigen>

#define DEG2RAD(x) ((x / 180.0) * M_PI)
#define SIGN(x) ((x < 0 ? -1 : 1))

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

}

#endif
