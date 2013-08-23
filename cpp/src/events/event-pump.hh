#ifndef EVENT_PUMP_HH
#define EVENT_PUMP_HH

#include "events/events.hh"

class EventPump {

public:
  virtual InputEventVector pump_events()=0;
};


#endif
