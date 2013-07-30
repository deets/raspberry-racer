#include "world.hh"


World::World(WindowAdapter &window_adapter, OpenVGAdapter &ovg_adapter)
  : _window_adapter(window_adapter)
  , _ovg_adapter(ovg_adapter)
{
}




void World::begin(const vector<Event> &events) {
  _window_adapter.start();
}


void World::end() {
  _window_adapter.end();
}
