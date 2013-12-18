#ifndef EVENT_PUMP_HH
#define EVENT_PUMP_HH

#include "events/events.hh"

class EventPump {

public:
  virtual GameEventVector pump_events()=0;
};


#endif
