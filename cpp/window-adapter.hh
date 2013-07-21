#ifndef WINDOW_SYSTEM_ADAPTER_HH
#define WINDOW_SYSTEM_ADAPTER_HH

#include <utility>

using namespace std;

class WindowAdapter {

public:
  virtual pair<int, int> window_dimensions()=0;
};

#endif
