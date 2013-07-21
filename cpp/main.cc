#include <iostream>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#include "rpi-adapter-impl.hh"
#include "terminal.hh"
#include "linux-event-pump.hh"
#include "rpi-window-adapter.hh"

using namespace std;

int main(int argc, char **argv) {
  RPiAdapterImpl rpi_adapter;
  Terminal terminal(rpi_adapter);
  terminal.install_signal_handler();

  LinuxEventPump event_pump(boost::bind(&Terminal::read_character, &terminal));

  RPiWindowAdapter window_adapter(rpi_adapter);

  bool running = true;
  while(running) {
    BOOST_FOREACH(const Event event, event_pump.pump_events()) {
      cout << "scancode: " << event.scancode << endl;
      if(event.key == K_ESC) {
	running = false;
      }
    }
  }
}
