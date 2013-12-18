#ifndef EVENTS_HH
#define EVENTS_HH

#include <vector>
#include <boost/variant.hpp>

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


  typedef boost::variant<KeyEvent> GameEvent;
  typedef vector<GameEvent> GameEventVector;

}; // end ns::rracer
#endif
