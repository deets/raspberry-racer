#ifndef RPI_WINDOW_ADAPTER_HH
#define RPI_WINDOW_ADAPTER_HH

#include <utility>
#include "GLES/gl.h"
#include "EGL/egl.h"
#include "window-adapter.hh"
#include "rpi-adapter.hh"
#include "non-copyable.hh"
#include "eglstate.h"

using namespace std;



class RPiWindowAdapter : public WindowAdapter, public NonCopyAble {

  uint32_t _width, _height;

  STATE_T _state;

public:
  RPiWindowAdapter();
  virtual ~RPiWindowAdapter();

  // WindowAdapter implementation
  virtual pair<int, int> window_dimensions();
  virtual void start();
  virtual void end();

  //virtual vgSetf (VGParamType type, VGfloat value);
  //virtual vgClear(VGint x, VGint y, VGint width, VGint height);
  //virtual void vgLoadIdentity();

};

#endif
