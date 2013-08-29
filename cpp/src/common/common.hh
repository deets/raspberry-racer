#ifndef COMMON_HH
#define COMMON_HH

#include <cassert>

#include <Eigen/Eigen>

namespace rracer {

 typedef Eigen::Affine2f AffineTransform;
  typedef Eigen::Vector2f Vector;
  typedef float Real;

  typedef struct {
    Vector origin;
    Vector size;
  } Rect;
}

#endif
