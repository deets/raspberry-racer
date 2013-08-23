#include <boost/foreach.hpp>

#include "world.hh"


void WorldObject::add_object(WorldObject* child) {
  _children.push_back(*child);
}


void WorldObject::dispatch_input_events(const InputEventVector& events) {
  this->process_input_events(events);

  BOOST_FOREACH(WorldObject& child, _children) {
    child.dispatch_input_events(events);
  }
}


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
    obj.dispatch_input_events(events);
  }
}


void World::end() {
  _window_adapter.end();
}


void World::add_object(WorldObject* obj) {
  _world_objects.push_back(*obj);
}

