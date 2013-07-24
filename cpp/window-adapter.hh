#ifndef WINDOW_SYSTEM_ADAPTER_HH
#define WINDOW_SYSTEM_ADAPTER_HH

#include "VG/openvg.h"
#include "VG/vgu.h"

#include <utility>

using namespace std;

class WindowAdapter {

public:
  virtual pair<int, int> window_dimensions()=0;
  virtual void start()=0;
  virtual void end()=0;

  // passing through OpenVG calls
  //  virtual vgSetf (VGParamType type, VGfloat value)=0;
  //virtual vgClear(VGint x, VGint y, VGint width, VGint height)=0;
  //virtual void vgLoadIdentity()=0;
};

#endif
