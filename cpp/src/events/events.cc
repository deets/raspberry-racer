#include "events/events.hh"

namespace rracer {

  TimeInfo::TimeInfo()
    : _when(0)
    , _elapsed(0)
  {
  }


  TimeInfo::TimeInfo(const Real when, const Real elapsed) 
    : _when(when)
    , _elapsed(elapsed)
  {
  }


  Real TimeInfo::when() const {
    return _when;
  }


  Real TimeInfo::elapsed() const {
    return _elapsed;
  }


  GameEvent::GameEvent(const TimeInfo& time_info, const Event& event) 
    : event(event)
    , time_info(time_info)
  {
  }
} // ns::rracer end
