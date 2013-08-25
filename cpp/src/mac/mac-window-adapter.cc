#include "mac/mac-window-adapter.hh"



MacWindowAdapter::MacWindowAdapter(int width, int height)
  : _width(width)
  , _height(height) 
{
}


MacWindowAdapter::~MacWindowAdapter() {
}

// WindowAdapter implementation

pair<int, int> MacWindowAdapter::window_dimensions() {
  return make_pair(_width, _height);
}


void MacWindowAdapter::start() {
}


void MacWindowAdapter::end() {

}


