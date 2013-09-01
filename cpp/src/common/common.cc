#include <stdexcept>
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

  Rect Rect::from_points(const vector<Vector>& points) {
      struct Local {
	static bool comp_x(const Vector& a, const Vector& b) { return a[0] < b[0]; }
	static bool comp_y(const Vector& a, const Vector& b) { return a[1] < b[1]; }
      };
      
      Real left = (*min_element(points.begin(), points.end(), &Local::comp_x))[0];
      Real right = (*max_element(points.begin(), points.end(), &Local::comp_x))[0];
      Real bottom = (*min_element(points.begin(), points.end(), &Local::comp_y))[1];
      Real top = (*max_element(points.begin(), points.end(), &Local::comp_y))[1];
      return Rect::from_corners(Vector(left, bottom), Vector(right, top));
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


  EQuarterClass classify_point(const Vector& cp, const Vector& point) {
    if(cp == point) {
      return EQ;
    }
    if(cp[1] == point[1] && cp[0] < point[0]) {
      return A0;
    }
    if(cp[0] < point[0] && cp[1] < point[1]) {
      return Q0;
    }
    if(cp[0] == point[0] && cp[1] < point[1]) {
      return A1;
    }
    if(cp[0] > point[0] && cp[1] < point[1]) {
      return Q1;
    }
    if(cp[0] > point[0] && cp[1] == point[1]) {
      return A2;
    }
    if(cp[0] > point[0] && cp[1] > point[1]) {
      return Q2;
    }
    if(cp[0] == point[0] && cp[1] > point[1]) {
      return A3;
    }
    if(cp[0] < point[0] && cp[1] > point[1]) {
      return Q3;
    }
  }


  Vector axis_point(const Vector& cp, const Real& radius, const EQuarterClass& axis) {
    switch(axis) {
    case A0:
      return Vector(cp[0] + radius, cp[1]);
    case A1:
      return Vector(cp[0], cp[1] + radius);
    case A2:
      return Vector(cp[0] - radius, cp[1]);
    case A3:
      return Vector(cp[0], cp[1] - radius);
    default:
      throw runtime_error("No axis passed!");
    }
  }
}; //ns::rracer
