#ifndef EVENTS_HH
#define EVENTS_HH

#include <vector>

using namespace std;

enum EKeys {
  K_ESC = 1,
  K_UNKNOWN = 0
};

struct InputEvent {
  bool pressed;
  EKeys key;
  int scancode;
};

typedef vector<InputEvent> InputEventVector;

#endif
