#include <iostream>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#include "assets/assets.hh"
#include "posix-adapter-impl.hh"
#include "terminal.hh"
#include "linux-event-pump.hh"
#include "rpi-window-adapter.hh"

using namespace std;

int main(int argc, char **argv) {
  PosixAdapterImpl posix_adapter;
  Terminal terminal(posix_adapter);
  terminal.install_signal_handler();

  LinuxEventPump event_pump(boost::bind(&Terminal::read_character, &terminal));

  RPiWindowAdapter window_adapter;

  AssetManager am(&window_adapter);
  
  bool running = true;
  while(running) {
    window_adapter.start();
    BOOST_FOREACH(const Event event, event_pump.pump_events()) {
      cout << "scancode: " << event.scancode << endl;
      if(event.key == K_ESC) {
	running = false;
      }
    }
    VGfloat black[4] = {.0, .0, .0, .1};
    window_adapter.setFillColor(black);
    window_adapter.drawCircle(100, 100, 100.0);
    am.drawText(500, 500, "Hallo!", am.get_font(), 30);
    window_adapter.end();
  }
}
