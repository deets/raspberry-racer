#ifndef LINUX_EVENT_PUMP_HH
#define LINUX_EVENT_PUMP_HH

#include <boost/function.hpp>

#include "events/event-pump.hh"

using namespace boost;

typedef function< int () > raw_terminal_code_callback;

class LinuxEventPump : public EventPump {

  raw_terminal_code_callback _read_raw_terminal_codes;

public:

  LinuxEventPump(raw_terminal_code_callback cb);

  virtual vector<Event> pump_events();

};

#endif
