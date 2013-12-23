#include <boost/foreach.hpp>
#include <boost/range.hpp>
#include <boost/range/algorithm_ext.hpp>
#include <math.h>

#include "scene/scene-node.hh"

namespace rracer {
  
  SceneNode::SceneNode() 
    : _name("")
    , _parent(NULL)
  {
  }


  void SceneNode::object_added(SceneNode* parent, SceneNode* child) {
    on_object_added(parent, child);
    if(this->parent()) {
      this->parent()->object_added(parent, child);
    }
  }


  void SceneNode::on_object_added(SceneNode* parent, SceneNode* child) {
  }


  string SceneNode::name() const {
    return _name;
  }


  void SceneNode::name(const string& n) {
    _name = n;
  }


  SceneNode* SceneNode::parent() const {
    return _parent;
  }


  void SceneNode::parent(SceneNode* p) {
    _parent = p;
  }


  void SceneNode::add_object(SceneNode* child) {
    child->parent(this);
    _children.push_back(*child);
    object_added(this, child);
  }


  SceneNode::iterator SceneNode::begin() {
    return _children.begin();
  }

  
  SceneNode::iterator SceneNode::end() {
    return _children.end();
  }


  void SceneNode::setup_within_world(b2World* ) {
  }


  void SceneNode::debug_render(DebugRenderer& debug_renderer) const {
  }


  void SceneNode::dispatch_frame_events(const GameEventVector& events, const TimeInfo& time_info, EventEmitter emit_event) {
    this->process_frame_events(events, time_info, emit_event);
    BOOST_FOREACH(SceneNode& child, _children) {
      child.dispatch_frame_events(events, time_info, emit_event);
    }
  }


  void SceneNode::dispatch_render(OpenVGCompanion& vgc) {
    AffineTransform t = vgc.current_matrix();
    this->render(vgc);
    BOOST_FOREACH(SceneNode& child, _children) {
      vgc.set(t);
      child.dispatch_render(vgc);
    }
  }

  void SceneNode::process_frame_events(const GameEventVector&, const TimeInfo& time_info, EventEmitter emit_event) {
  }


  void SceneNode::render(OpenVGCompanion& vgc) const {
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


  void LissajouAnimator::process_frame_events(const GameEventVector& events, const TimeInfo& time_info, EventEmitter emit_event) {
    _phase += time_info.elapsed();
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
    SceneNode::dispatch_render(vgc);
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


  void KeyAction::process_frame_events(const GameEventVector& events, const TimeInfo& time_info, EventEmitter emit_event) {
    BOOST_FOREACH(const GameEvent& event, events) {
      KeyEvent key_event = boost::get<KeyEvent>(event.event);
      if(key_event.pressed && key_event.key == _key) {
	_key_callback();
      }
    }
  }
  
}; // ns::rracer
