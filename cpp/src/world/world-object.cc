#include <boost/foreach.hpp>
#include <math.h>

#include "world-object.hh"

namespace rracer {

  void WorldObject::add_object(WorldObject* child) {
    _children.push_back(*child);
  }


  WorldObject::wo_iterator WorldObject::begin() {
    return _children.begin();
  }

  
  WorldObject::wo_iterator WorldObject::end() {
    return _children.end();
  }


  void WorldObject::physics_setup(b2World* ) {
  }


  void WorldObject::debug_render(DebugRenderer& debug_renderer) const {
  }


  void WorldObject::dispatch_input_events(const InputEventVector& events, double elapsed) {
    this->process_input_events(events, elapsed);

    BOOST_FOREACH(WorldObject& child, _children) {
      child.dispatch_input_events(events, elapsed);
    }
  }


  void WorldObject::dispatch_render(OpenVGCompanion& vgc) {
    this->render(vgc);

    BOOST_FOREACH(WorldObject& child, _children) {
      child.dispatch_render(vgc);
    }
  }

  void WorldObject::process_input_events(const InputEventVector&, double elapsed) {
  }


  void WorldObject::render(OpenVGCompanion& vgc) const {
  }


  Image::Image(AssetManager& am, string asset_name) 
    : _am(am)
    , _asset_name(asset_name)
  {
  }


  void Image::render(OpenVGCompanion& vgc) const {
    const PNGImageData& img_data = _am.image(_asset_name);
    VGfloat w = (VGfloat)img_data.width();
    VGfloat h = (VGfloat)img_data.height();
    vgc.vg().vgRotate(10);
    vgc.vg().vgTranslate(-w / 2.0, -h / 2.0);
    vgc.drawImage(img_data);
  }


  Translator::Translator(float x, float y) 
    : _x(x)
    , _y(y)
  {

  }

  void Translator::render(OpenVGCompanion& vgc) const {
    vgc.vg().vgTranslate(_x, _y);
  }


  LissajouAnimator::LissajouAnimator(float width, float height, float alpha, float beta)
    : _width(width)
    , _height(height)
    , _alpha(alpha)
    , _beta(beta)
    , _phase(.0)
  {

  }


  void LissajouAnimator::process_input_events(const InputEventVector& events, double elapsed) {
    _phase += elapsed;
  }


  void LissajouAnimator::render(OpenVGCompanion& vgc) const {
    float w = sin(_phase * _alpha) * _width / 2.0;
    float h = sin(_phase * _beta) * _height / 2.0;
    vgc.vg().vgTranslate(w, h);
  }



  AffineTransformator::AffineTransformator(const AffineTransform& t)
    : _t(t)
  {
  }


  AffineTransform AffineTransformator::affine_transform() const {
    return _t;
  }


  void AffineTransformator::dispatch_render(OpenVGCompanion& vgc) {
    MatrixStacker ms(vgc, _t);
    WorldObject::dispatch_render(vgc);
  }


  CircleRenderer::CircleRenderer(function< Vector () > pc, Real radius, const Color& color) 
    : _position_callback(pc)
    , _radius(radius)
    , _color(color) 
  {
  }


  void CircleRenderer::render(OpenVGCompanion& vgc) const {
    PaintScope p(vgc, _color, VG_FILL_PATH | VG_STROKE_PATH);
    vgc.set();
    vgc.stroke_width(1.0);
    const Vector pos = _position_callback();
    vgc.drawCircle(pos[0], pos[1], _radius);
  }


  // =======================================================================

  KeyAction::KeyAction(EKeys key, function< void () > key_callback) 
    : _key(key)
    , _key_callback(key_callback)
  {
  }


  void KeyAction::process_input_events(const InputEventVector& events, double elapsed) {
    BOOST_FOREACH(const InputEvent& event, events) {
      if(event.pressed && event.key == _key) {
	_key_callback();
      }
    }
  }
  
}; // ns::rracer
