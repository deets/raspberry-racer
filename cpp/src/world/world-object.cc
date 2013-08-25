#include <boost/foreach.hpp>
#include <math.h>

#include "world-object.hh"


void WorldObject::add_object(WorldObject* child) {
  _children.push_back(*child);
}


void WorldObject::dispatch_input_events(const InputEventVector& events, double elapsed) {
  this->process_input_events(events, elapsed);

  BOOST_FOREACH(WorldObject& child, _children) {
    child.dispatch_input_events(events, elapsed);
  }
}


void WorldObject::dispatch_render(const OpenVGCompanion& vgc) {
  this->render(vgc);

  BOOST_FOREACH(WorldObject& child, _children) {
    child.dispatch_render(vgc);
  }
}


void WorldObject::process_input_events(const InputEventVector&, double elapsed) {
}



void WorldObject::render(const OpenVGCompanion& vgc) const {

}


Image::Image(AssetManager& am, string asset_name) 
  : _am(am)
  , _asset_name(asset_name)
{
}


void Image::render(const OpenVGCompanion& vgc) const {
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

void Translator::render(const OpenVGCompanion& vgc) const {
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


void LissajouAnimator::render(const OpenVGCompanion& vgc) const {
  float w = sin(_phase * _alpha) * _width / 2.0;
  float h = sin(_phase * _beta) * _height / 2.0;
  vgc.vg().vgTranslate(w, h);
}
