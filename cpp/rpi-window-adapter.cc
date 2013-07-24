#include <assert.h>
#include <bcm_host.h>

#include "rpi-window-adapter.hh"



using namespace std;

RPiWindowAdapter::RPiWindowAdapter()
{
  ::bcm_host_init();
  ::memset(&_state, 0, sizeof(_state));
  ::oglinit(&_state);
  ::graphics_get_display_size(0, &_width, &_height);
}


RPiWindowAdapter::~RPiWindowAdapter() {
  ::bcm_host_deinit();
}


pair<int, int> RPiWindowAdapter::window_dimensions() {
  return make_pair((int)_width, (int)_height);
}

void RPiWindowAdapter::start() {
  VGfloat color[4] = { 255, 255, 255, 1 };
  ::vgSetfv(VG_CLEAR_COLOR, 4, color);
  ::vgClear(0, 0, _width, _height);
  color[0] = 0, color[1] = 0, color[2] = 0;
  //setfill(color);
  //setstroke(color);
  //StrokeWidth(0);
  ::vgLoadIdentity();
}

void RPiWindowAdapter::end() {
  assert(::vgGetError() == VG_NO_ERROR);
  ::eglSwapBuffers(_state.display, _state.surface);
  assert(::eglGetError() == EGL_SUCCESS);
}
