#include "rpi-window-adapter.hh"


using namespace std;

RPiWindowAdapter::RPiWindowAdapter(RPiAdapter &rpi) :
  _rpi(rpi)
{
  _rpi.bcm_host_init();
  _rpi.graphics_get_display_size(0, &_width, &_height);
}


RPiWindowAdapter::~RPiWindowAdapter() {
  _rpi.bcm_host_deinit();
}


pair<int, int> RPiWindowAdapter::window_dimensions() {
  return make_pair((int)_width, (int)_height);
}
