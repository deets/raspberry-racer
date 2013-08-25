#include <boost/foreach.hpp>

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
  vgc.drawImage(img_data, 0, 0);
}
