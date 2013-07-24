#include "world.hh"


World::World(WindowAdapter &window_adapter) :
  _window_adapter(window_adapter)
{
  _window_dimensions = window_adapter.window_dimensions();
}




void World::begin(const vector<Event> &events) {
  _window_adapter.start();
}


void World::end() {
  _window_adapter.end();
}
