#include <iostream>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

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

  RPiWindowAdapter rpi_adapter;

  bool running = true;
  while(running) {
    rpi_adapter.start();
    BOOST_FOREACH(const Event event, event_pump.pump_events()) {
      cout << "scancode: " << event.scancode << endl;
      if(event.key == K_ESC) {
	running = false;
      }
    }
    VGfloat black[4] = {.0, .0, .0, .1};
    rpi_adapter.setFillColor(black);
    rpi_adapter.drawCircle(100, 100, 100.0);
    rpi_adapter.end();
  }
}
