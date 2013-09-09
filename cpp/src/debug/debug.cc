#include <cassert>

#include "debug/debug.hh"

namespace rracer {
  DebugRenderer::DebugRenderer(OpenVGAdapter& vg) 
    : _world_transform(AffineTransform::Identity)
    , _alpha(0.3)
  {
    _vgc = new OpenVGCompanion(vg);
  }

  void DebugRenderer::world_transform(function<AffineTransform ()> tf) {
    _world_transform = tf;
  }


  DebugRenderer::~DebugRenderer() {
    delete _vgc;
  }

  void DebugRenderer::render(b2World* world) {
    world->SetDebugDraw(this);
    world->DrawDebugData();
    world->SetDebugDraw(NULL);
  }

  void DebugRenderer::draw_polygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color, const VGbitfield fill_type) {
    MatrixStacker(*_vgc, _world_transform());
    _vgc->stroke_width(1.0);
    PaintScope paint(*_vgc, Color(color.r, color.g, color.b, _alpha), fill_type);
    PathScope p(*_vgc, fill_type);
    assert(vertexCount > 1);
    _vgc->move_to(p, vconv(vertices[0]), VG_ABSOLUTE);
    for(int i = 1; i < vertexCount; ++i) {
      _vgc->line_to(p, vconv(vertices[i]), VG_ABSOLUTE);
    }
    _vgc->close(p);
  }


  void DebugRenderer::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    draw_polygon(vertices, vertexCount, color, VG_STROKE_PATH);
  }


  void DebugRenderer::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    draw_polygon(vertices, vertexCount, color, VG_FILL_PATH);
  }


  void DebugRenderer::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {
    MatrixStacker(*_vgc, _world_transform());
    PaintScope paint(*_vgc, Color(color.r, color.g, color.b, _alpha), VG_STROKE_PATH);
    _vgc->stroke_width(1.0);
    VGPath path = _vgc->newPath();
    _vgc->vg().vguEllipse(path, center.x, center.y, radius, radius);
    _vgc->vg().vgDrawPath(path, VG_STROKE_PATH);
    _vgc->vg().vgDestroyPath(path);
  }


  void DebugRenderer::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {
    MatrixStacker(*_vgc, _world_transform());
    PaintScope paint(*_vgc, Color(color.r, color.g, color.b, _alpha), VG_FILL_PATH);
    _vgc->stroke_width(1.0);
    VGPath path = _vgc->newPath();
    _vgc->vg().vguEllipse(path, center.x, center.y, radius, radius);
    _vgc->vg().vgDrawPath(path, VG_FILL_PATH);
    _vgc->vg().vgDestroyPath(path);
  }


  void DebugRenderer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {}
  void DebugRenderer::DrawTransform(const b2Transform& xf) {}

}; // end ns::rracer
