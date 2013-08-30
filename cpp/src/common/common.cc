#include "common/common.hh"

namespace rracer {
  AffineTransform rotation(int degree) {
    AffineTransform t = AffineTransform::Identity();
    return t.rotate(DEG2RAD(degree));
  }

}; //ns::rracer
