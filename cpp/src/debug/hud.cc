#include <boost/foreach.hpp>
#include "debug/hud.hh"

namespace rracer {
  HUD::HUD(const Vector& position,  const Fontinfo& fi, World& world, function<AffineTransform ()> world_transform) 
    : _position(position)
    , _fi(fi)
    , _world(world)
    , _visible(true)
    , _debug_renderer_set(true)
  {
    _debug_renderer = new rracer::DebugRenderer(_world.vg(), .1);
    _debug_renderer->SetFlags(b2Draw::e_shapeBit | b2Draw::e_centerOfMassBit);
    _debug_renderer->world_transform(world_transform);
    _world.set_debug_renderer(_debug_renderer);
  }

  HUD::~HUD() {
    _world.set_debug_renderer(0);
    delete _debug_renderer;
  }

  void HUD::process_input_events(const InputEventVector& events, double elapsed) {
    BOOST_FOREACH(const InputEvent event, events) {
      if(!event.pressed) {
	switch(event.key) {
	case K_h:
	  _visible = !_visible;
	  break;
	case K_d:
	  if(_debug_renderer_set) {
	    _world.set_debug_renderer(0);
	  } else {
	    _world.set_debug_renderer(_debug_renderer);
	  }
	  _debug_renderer_set = !_debug_renderer_set;
	  break;
	}
      }
    }
  }


  void HUD::render(OpenVGCompanion& vgc) const {
    if(!_visible) {
      return;
    }
    MatrixStacker ms(vgc, AffineTransform::Identity());    
    PaintScope text_color(vgc, Color::yellow, VG_FILL_PATH);
    VGfloat pos = _position[1];
    vgc.drawText(_fi, _position[0], pos, "HUD", 10);
    const char *preamble[] = {
      "H - toggle HUD",
      "D - toggle Debug Draw",
    };
    pos -= 12;
    for(int i = 0; i < 2; ++i) {
      vgc.drawText(_fi, _position[0], pos, (char*)preamble[i], 10);
      pos -= 12;
    }
  }
  
}
