#ifndef MAC_WINDOW_ADAPTER
#define MAC_WINDOW_ADAPTER

#include "gfx/window-adapter.hh"
#include "gfx/openvg-adapter-impl.hh"
#include "common/non-copyable.hh"

using namespace std;

class MacWindowAdapter : public WindowAdapter, public OpenVGAdapterImpl, public NonCopyAble {
  int _width, _height;

public:
  MacWindowAdapter(int width, int height);
  virtual ~MacWindowAdapter();
  // WindowAdapter implementation
  virtual pair<int, int> window_dimensions();
  virtual void start();
  virtual void end();
};


#endif
