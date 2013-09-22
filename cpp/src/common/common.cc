#include <stdexcept>
#include <map>
#include <algorithm>
#include "common/common.hh"
#include <boost/assign/list_of.hpp>

using namespace boost::assign;
using namespace std;

namespace rracer {

  AffineTransform rotation(int degree) {
    AffineTransform t = AffineTransform::Identity();
    return t.rotate(DEG2RAD(degree));
  }

  AffineTransform scale(Real f) {
    AffineTransform t = AffineTransform::Identity();
    return t.scale(f);
  }

  AffineTransform translate(const Vector& v) {
    AffineTransform t = AffineTransform::Identity();
    return t.translate(v);
  }

  Vector vconv(const b2Vec2& v) {
    return Vector(v.x, v.y);
  }


  b2Vec2 vconv(const Vector& v) {
    return b2Vec2(v[0], v[1]);
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


  // Rect Rect::operator=(const Rect& other) const {
  //   return Rect(other.origin, other.size);
  // }

  Real Rect::left() const { return origin[0]; }
  Real Rect::right() const { return origin[0] + size[0];}

  Real Rect::width() const { return size[0]; }
  Real Rect::height() const { return size[1]; }
  Real Rect::ratio() const { return size[1] / size[0]; }
  

  // we have a bottom-left origin
  Real Rect::top() const { return origin[1] + size[1]; }
  Real Rect::bottom() const { return origin[1]; }

  Vector Rect::center() const {
    return Vector((right() + left()) / 2, (top() + bottom()) / 2);
  }

  Rect Rect::operator*(Real f) const {
    return Rect::from_center_and_size(center(), size * f);
  }

  Rect Rect::from_center_and_size(const Vector& center, const Vector& size) {
    const Vector sh = size / 2.0;
    return Rect(center - sh, size);
  }


  bool Rect::contains(const Vector& p) const {
    return left() <= p[0] && right() > p[0] && bottom() <= p[1] && top() > p[1];
  }

  AffineTransform Rect::fit(const Rect& rect) const {
    Real sf;
    // when we have a larger ration than
    // our fittling, we are "slimmer" - so
    // the scaling operates on the width
    if(ratio() >= rect.ratio()) {
      sf = width() / rect.width();
    } else {
      sf = height() / rect.height();
    }
    return translate(center()) * scale(sf) * translate(-rect.center());
  }

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


  vector<EQuarterClass> spanning_quadrants(EQuarterClass q1, EQuarterClass q2) {
    static vector<EQuarterClass> s_ccw_quarters = list_of(Q0)(Q1)(Q2)(Q3)(Q0)(Q1)(Q2)(Q3);
    static vector<EQuarterClass> s_ccw_axis =     list_of(A1)(A2)(A3)(A0)(A1)(A2)(A3)(A0);
    static map<EQuarterClass, EQuarterClass> s_q1_axis_to_quarter = map_list_of(A0, Q0)(A1, Q1)(A2, Q2)(A3, Q3);
    static map<EQuarterClass, EQuarterClass> s_q2_axis_to_quarter = map_list_of(A0, Q3)(A1, Q0)(A2, Q1)(A3, Q2);
    vector<EQuarterClass> res;

    if(s_q1_axis_to_quarter.find(q1) != s_q1_axis_to_quarter.end()) {
      q1 = s_q1_axis_to_quarter[q1];
    }
    if(s_q2_axis_to_quarter.find(q2) != s_q2_axis_to_quarter.end()) {
      q2 = s_q2_axis_to_quarter[q2];
    }
    
    bool found = false;
    for(int i = 0; i < 8; i++) {
      if(!found) {
	if(s_ccw_quarters[i] == q1) {
	  found = true;
	  res.push_back(s_ccw_axis[i]);
	}
      } else {
	if(s_ccw_quarters[i] == q2) {
	  break;
	} else {
	  res.push_back(s_ccw_axis[i]);
	}
      }
    }
    
    return res;
  }

  //=========================================

  Color::Color(VGfloat red, VGfloat green, VGfloat blue, VGfloat alpha)
  {
    _components[0] = red;
    _components[1] = green;
    _components[2] = blue;
    _components[3] = alpha;
  }

  Color::operator const VGfloat*() const {
    return _components;
  }

  // Static color definitions
  const Color Color::black = Color(0, 0, 0);
  const Color Color::yellow = Color(1, 1, 0);

}; //ns::rracer
