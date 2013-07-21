#ifndef RPI_WINDOW_ADAPTER_HH
#define RPI_WINDOW_ADAPTER_HH

#include <utility>

#include "window-adapter.hh"
#include "rpi-adapter.hh"

using namespace std;

class RPiWindowAdapter : public WindowAdapter {

  RPiAdapter &_rpi;

  uint32_t _width, _height;

public:
  RPiWindowAdapter(RPiAdapter &rpi);
  virtual ~RPiWindowAdapter();

  // WindowAdapter implementation
  virtual pair<int, int> window_dimensions();

};

#endif
