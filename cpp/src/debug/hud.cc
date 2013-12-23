#include <sstream>
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

  void HUD::process_frame_events(const GameEventVector& events, const TimeInfo& time_info, EventEmitter emit_event) {
    _time_info = time_info;
    BOOST_FOREACH(const GameEvent event, events) {
      KeyEvent key_event = boost::get<KeyEvent>(event.event);
      if(!key_event.pressed) {
	switch(key_event.key) {
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
    std::stringstream lines;
    lines << "H - toggle HUD" << std::endl;
    lines << "D - toggle Debug Draw" << std::endl;
    lines << "time: " << _time_info.when() << std::endl;
    pos -= 12;
    while(!lines.eof()) {
      std::string line;
      std::getline(lines, line);
      vgc.drawText(_fi, _position[0], pos, line.c_str(), 10);
      pos -= 12;
    }
  }
  
}
