#ifndef COMMON_HH
#define COMMON_HH

#include <vector>
#include <cassert>
#include <math.h>
#include <VG/openvg.h>
#include <Box2D/Box2D.h>
#include <Eigen/Eigen>

#define DEG2RAD(x) ((x / 180.0) * M_PI)
#define RAD2DEG(x) ((x / M_PI) * 180.0)
#define SIGN(x) ((x < 0 ? -1 : 1))


using namespace std;

namespace rracer {

  typedef Eigen::Affine2d AffineTransform;
  typedef Eigen::Vector2d Vector;
  typedef double Real;

  AffineTransform rotation(int degree);
  AffineTransform scale(Real f);
  AffineTransform translate(const Vector&);

  typedef struct {
    Vector point;
    Real direction;
  } ConnectionPoint;

  class Rect {

  public:
    Vector origin;
    Vector size;

    Rect();
    Rect(const Vector& origin, const Vector& size);
    Rect(Real x, Real y, Real width, Real height);
    static Rect from_corners(const Vector& bottom_left, const Vector& top_right);
    static Rect from_points(const vector<Vector>&);
    static Rect from_center_and_size(const Vector& center, const Vector& size);
    virtual ~Rect();

    Real left() const;
    Real right() const;
    Real top() const;
    Real bottom() const;
    Real width() const;
    Real height() const;
    Real ratio() const;
    Vector center() const;
    bool empty() const;
    bool contains(const Vector&) const;

    bool operator==(const Rect&) const;
    Rect operator|(const Rect&) const;
    Rect operator*(Real) const;
    //    Rect operator=(const Rect&) const;

    /**
     * Returns a transformation so that
     * the passed rect is scaled and
     * translated to fit "this"
     */
    AffineTransform fit(const Rect& rect) const;

  };

  class Color {

    VGfloat _components[4];
  public:
    Color(VGfloat red, VGfloat green, VGfloat blue, VGfloat alpha=1.0);
    operator const VGfloat*() const;

    static const Color black;
    static const Color yellow;
    static const Color red;
  };


  Vector vconv(const b2Vec2&);
  b2Vec2 vconv(const Vector&);

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


  /**
   * For two quadrants, enumerate all axis spanned by them.
   * The function assumes always counter clockwise traversal
   */
  vector<EQuarterClass> spanning_quadrants(EQuarterClass q1, EQuarterClass q2);
}

#endif
