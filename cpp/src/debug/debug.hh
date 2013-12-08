#ifndef DEBUG_HH
#define DEBUG_HH

#include <Box2D/Box2D.h>
#include <boost/function.hpp>

#include "common/common.hh"
#include "gfx/openvg-companion.hh"

using namespace boost;

namespace rracer {
  class DebugRenderer : public b2Draw, public NonCopyAble {
  public:
    DebugRenderer(OpenVGAdapter&, Real line_width);
    virtual ~DebugRenderer();

    void world_transform(function<AffineTransform ()>);

    void render(b2World* world);
    void draw_polygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color, const VGbitfield fill_type);

    virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
    virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
    virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
    virtual void DrawTransform(const b2Transform& xf);

    void render_arrow(const b2Vec2& p, const b2Vec2& d, const Color& color);

  private:
    OpenVGCompanion* _vgc;

    function<AffineTransform ()> _world_transform;

    Real _alpha;
    Real _line_width;

  };

}; // end ns::rracer
#endif
