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

  typedef struct {
    Vector origin;
    Vector size;
  } Rect;

  AffineTransform rotation(int degree);

}

#endif
