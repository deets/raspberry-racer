#include <errno.h>
#include <string.h>
#include <string>
#include <stdexcept>
#include <linux/kd.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <boost/foreach.hpp>
#include <signal.h>

#include "terminal.hh"

static Terminal *the_terminal;

Terminal::Terminal() {
  if(tcgetattr(STDIN_FILENO, &_terminal_settings)) {
    throw runtime_error(strerror(errno));
  }
  struct termios raw_settings;
  raw_settings = _terminal_settings;
  raw_settings.c_lflag = ~(ECHO | ICANON | IEXTEN | ISIG);
  if(tcsetattr(STDIN_FILENO, TCSANOW, &raw_settings)) {
    throw runtime_error(strerror(errno));
  }
  
  if(ioctl(STDIN_FILENO, KDSKBMODE, K_RAW)) {
    throw runtime_error(strerror(errno));
  }
  _resetters.push_back(boost::mem_fn(&Terminal::reset_raw_kb_mode));
}

void Terminal::reset_raw_kb_mode() {
  if(ioctl(STDIN_FILENO, KDSKBMODE, K_XLATE)) {
    throw runtime_error(strerror(errno));
  }
}

void Terminal::reset() {
  if(_resetters.size()) {
    BOOST_REVERSE_FOREACH(std::tr1::function<void (Terminal*) > f, _resetters) {
      f(this);
    }
    _resetters.clear();
    tcsetattr(STDIN_FILENO, TCSANOW, &_terminal_settings);
  }
}


Terminal::~Terminal() {
  reset();
}


int Terminal::read_character() {
  struct timeval timeout = {0, 0};
  fd_set rdfs;
  FD_ZERO(&rdfs);
  FD_SET(STDIN_FILENO, &rdfs);  
  select(1, &rdfs, NULL, NULL, &timeout);
  if(FD_ISSET(STDIN_FILENO, &rdfs)) {
    char buf[1];
    read(STDIN_FILENO, (void*)buf, sizeof(buf));
    return (buf[0] & 0x7f);
  }
  return -1;
}

void reset_terminal(int s) {
  the_terminal->reset();
}

void Terminal::install_signal_handler() {
  the_terminal = this;
  signal(SIGTERM, reset_terminal);
}
