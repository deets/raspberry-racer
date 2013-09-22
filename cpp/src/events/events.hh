#ifndef EVENTS_HH
#define EVENTS_HH

#include <vector>

using namespace std;

namespace rracer {

  enum EKeys {
    K_UNKNOWN = 0,
    K_ESC = 53,
    K_d = 2,
    K_h = 4,
    K_UP = 126,
    K_DOWN = 125,
    K_LEFT = 123,
    K_RIGHT = 124,
    K_SPACE = 49
  };

  struct InputEvent {
    bool pressed;
    EKeys key;
    int scancode;
  };

typedef vector<InputEvent> InputEventVector;

}; // end ns::rracer
#endif
