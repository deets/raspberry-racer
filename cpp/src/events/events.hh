#ifndef EVENTS_HH
#define EVENTS_HH

#include <vector>
#include <boost/variant.hpp>
#include <boost/function.hpp>

#include "common/common.hh"

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

  struct KeyEvent {
    KeyEvent()
      : pressed(false)
      , key(K_UNKNOWN)
      , scancode(0)
    {
    }
    KeyEvent(bool pressed, EKeys key, int scancode)
      : pressed(pressed)
      , key(key)
      , scancode(scancode)
    {
    }
    bool pressed;
    EKeys key;
    int scancode;
  };

  class TimeInfo {

  public:
    TimeInfo();
    TimeInfo(const Real when, const Real elapsed);
    //    TimeInfo(const TimeInfo& other);
    Real when() const;
    Real elapsed() const;

  private:
    Real _when, _elapsed;
  };

  struct HUDStringEvent {
    std::string payload;
  };


  class CarInfoEvent {

  };

  typedef boost::variant<KeyEvent, HUDStringEvent, CarInfoEvent> Event;

  class GameEvent {
  public:
    GameEvent(const TimeInfo& time_info, const Event& event);
    Event event;
    TimeInfo time_info;

  };

  typedef vector<GameEvent> GameEventVector;
  typedef boost::function<void (const GameEvent&)> EventEmitter;
}; // end ns::rracer
#endif
