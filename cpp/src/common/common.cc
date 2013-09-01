#include <algorithm>
#include "common/common.hh"

using namespace std;

namespace rracer {
  AffineTransform rotation(int degree) {
    AffineTransform t = AffineTransform::Identity();
    return t.rotate(DEG2RAD(degree));
  }


  Rect::Rect() 
    : origin(0, 0)
    , size(0, 0)
  {
  }

  Rect::Rect(const Vector& origin, const Vector& size)
    : origin(origin)
    , size(size)
  {
  }

  Rect::Rect(Real x, Real y, Real width, Real height)
    : origin(x, y)
    , size(width, height)
  {
  }

  Rect::~Rect() {
  }

  bool Rect::empty() const {
    return size == Vector(0, 0);
  }

  bool Rect::operator==(const Rect& other) const {
    return this->origin == other.origin && this->size == other.size;
  }

  Rect Rect::from_corners(const Vector& bottom_left, const Vector& top_right) {
    return Rect(bottom_left, top_right - bottom_left);
  }


  Rect Rect::operator|(const Rect& other) const {
    if(empty()) {
      return other;
    } else if(other.empty()) {
      return *this;
    } else {
      return Rect::from_corners(
	  Vector(min(this->left(), other.left()), min(this->bottom(), other.bottom())),
	  Vector(max(this->right(), other.right()), max(this->top(), other.top()))
      );
    }
  }


  Real Rect::left() const { return origin[0]; }
  Real Rect::right() const { return origin[0] + size[0];}
  // we have a bottom-left origin
  Real Rect::top() const { return origin[1] + size[1]; }
  Real Rect::bottom() const { return origin[1]; }
  
}; //ns::rracer
