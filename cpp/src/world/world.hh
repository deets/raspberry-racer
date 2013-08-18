#ifndef WORLD_HH
#define WORLD_HH

#include "common/common.hh"
#include "gfx/window-adapter.hh"
#include "gfx/openvg-adapter.hh"
#include "events/event-pump.hh"

class WorldObject {

public:

  virtual ~WorldObject() {}
};


class World {

  WindowAdapter &_window_adapter;
  OpenVGAdapter &_ovg_adapter;

public:
  World(WindowAdapter &, OpenVGAdapter &);

  void begin(const vector<Event> &events);
  void end();
};


#endif
