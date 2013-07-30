#ifndef WORLD_HH
#define WORLD_HH

#include "window-adapter.hh"
#include "openvg-adapter.hh"
#include "event-pump.hh"

class World {

  WindowAdapter &_window_adapter;
  OpenVGAdapter &_ovg_adapter;

public:
  World(WindowAdapter &, OpenVGAdapter &);

  void begin(const vector<Event> &events);
  void end();
};


#endif
