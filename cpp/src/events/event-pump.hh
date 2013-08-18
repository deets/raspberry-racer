#ifndef EVENT_PUMP_HH
#define EVENT_PUMP_HH

#include <vector>

using namespace std;

enum EKeys {
  K_ESC = 1,
  K_UNKNOWN = 0
};

struct Event {
  bool pressed;
  EKeys key;
  int scancode;
};


class EventPump {

public:
  virtual vector<Event> pump_events()=0;
};


#endif
