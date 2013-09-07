#include <boost/foreach.hpp>

#include "gfx/openvg-companion.hh"
#include "world.hh"


namespace rracer {

  World::World(WindowAdapter &window_adapter, OpenVGAdapter &ovg_adapter)
    : _window_adapter(window_adapter)
    , _ovg_adapter(ovg_adapter)
    , _has_ended(false)
  {
  }




  void World::begin(const InputEventVector& events, float elapsed) {
    _window_adapter.start();

    BOOST_FOREACH(const InputEvent event, events) {
      if(event.key == K_ESC) {
	_has_ended = true;
      }
    }

    BOOST_FOREACH(WorldObject& obj, _world_objects) {
      obj.dispatch_input_events(events, elapsed);
    }

    OpenVGCompanion vgc(_ovg_adapter);
  
    VGint ww = _window_adapter.window_dimensions().first;
    VGint wh = _window_adapter.window_dimensions().second;
    VGfloat black[] = {0.0,0,0,1.0};
    VGfloat red[] = {1.0,0,0,1.0};
    _ovg_adapter.vgSetfv(VG_CLEAR_COLOR, 4, black);
    _ovg_adapter.vgClear(0, 0, ww, wh);


    _ovg_adapter.vgLoadIdentity();

    BOOST_FOREACH(WorldObject& obj, _world_objects) {
      obj.dispatch_render(vgc);
    }
  }


  void World::end() {
    _window_adapter.end();
  }


  void World::add_object(WorldObject* obj) {
    _world_objects.push_back(*obj);
  }

}; // ns::rracer
