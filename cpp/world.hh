#ifndef WORLD_HH
#define WORLD_HH

#include "window-adapter.hh"
#include "event-pump.hh"

class World {

  WindowAdapter &_window_adapter;
  pair<int, int> _window_dimensions;

public:
  World(WindowAdapter &window_adapter);

  void begin(const vector<Event> &events);
  void end();
};


#endif
