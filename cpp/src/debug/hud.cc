#include <boost/foreach.hpp>
#include "debug/hud.hh"

namespace rracer {
  HUD::HUD(const Vector& position,  const Fontinfo& fi, OpenVGAdapter& vg, World* world, function<AffineTransform ()> world_transform) 
    : _position(position)
    , _fi(fi)
    , _world(world)
    , _visible(true)
  {
    _debug_renderer = new rracer::DebugRenderer(vg);
    _debug_renderer->SetFlags(b2Draw::e_shapeBit | b2Draw::e_centerOfMassBit);
    _debug_renderer->world_transform(world_transform);
    _world->set_debug_renderer(_debug_renderer);
  }

  HUD::~HUD() {
    _world->set_debug_renderer(0);
    delete _debug_renderer;
  }

  void HUD::process_input_events(const InputEventVector& events, double elapsed) {
    BOOST_FOREACH(const InputEvent event, events) {
      if(!event.pressed && event.key == K_h) {
	_visible = !_visible;
      }
    }
  }


  void HUD::render(OpenVGCompanion& vgc) const {
    if(!_visible) {
      return;
    }
    MatrixStacker ms(vgc, AffineTransform::Identity());    
    PaintScope text_color(vgc, Color::yellow, VG_FILL_PATH);
    vgc.drawText(_fi, _position[0], _position[1], "HUD", 10);
  }
  
}
