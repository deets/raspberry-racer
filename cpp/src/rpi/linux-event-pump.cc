#include "linux-event-pump.hh"


LinuxEventPump::LinuxEventPump(raw_terminal_code_callback cb)
  :
  _read_raw_terminal_codes(cb)
{
}


InputEventVector LinuxEventPump::pump_events() {
  InputEventVector res;
  while(true) {
    int c = _read_raw_terminal_codes();
    if(c == -1) {
      break;
    }
    InputEvent e;
    e.pressed = (c & 0x80) > 0;
    e.scancode = (c & 0x7f);
    
    switch(e.scancode) {
    case 1:
      e.key = K_ESC;
      break;
    default:
      e.key = K_UNKNOWN;
    }
    res.push_back(e);
  }
  return res;
}
