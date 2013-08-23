#ifndef WORLD_HH
#define WORLD_HH

#include <boost/intrusive/list.hpp>


#include "common/common.hh"
#include "gfx/window-adapter.hh"
#include "gfx/openvg-adapter.hh"
#include "events/events.hh"

namespace bi = boost::intrusive;


class World;

class WorldObject : public bi::list_base_hook<> {

public:
  typedef bi::list<WorldObject> WorldObjectList;

  virtual ~WorldObject() {}

  virtual void process_input_events(const InputEventVector&)=0;
  void add_object(WorldObject *obj);

  friend class World;

private:
  
  void dispatch_input_events(const InputEventVector&);

  WorldObjectList _children;
};



class World {
  bool _has_ended;

  WindowAdapter &_window_adapter;
  OpenVGAdapter &_ovg_adapter;

  WorldObject::WorldObjectList _world_objects;

public:
  World(WindowAdapter &, OpenVGAdapter &);

  void begin(const InputEventVector &events, float elapsed);
  void end();
  bool has_ended() { return _has_ended; };

  void add_object(WorldObject* child);
};


#endif
