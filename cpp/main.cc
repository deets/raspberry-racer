#include <iostream>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#include "assets/assets.hh"
#include "rpi/posix-adapter-impl.hh"
#include "rpi/terminal.hh"
#include "rpi/linux-event-pump.hh"
#include "rpi/rpi-window-adapter.hh"

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
    
    fs::path image_path("amiga-ball.png");
    ImageInfo img = am.image(image_path);
    window_adapter.vgSetPixels(200, 200, img.image, 0, 0, img.width, img.height);
    window_adapter.vgDestroyImage(img.image);
    window_adapter.end();
  }
}
