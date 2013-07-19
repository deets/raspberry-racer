#include <errno.h>
#include <string.h>
#include <string>
#include <stdexcept>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <boost/foreach.hpp>
#include <signal.h>

#include "terminal.hh"

Terminal *Terminal::s_signal_handler_terminal;

Terminal::Terminal(PosixAdapter &adapter) :
  _p(adapter)
{
  if(_p.tcgetattr(STDIN_FILENO, &_terminal_settings)) {
    throw runtime_error(strerror(errno));
  }
  struct termios raw_settings;
  raw_settings = _terminal_settings;
  raw_settings.c_lflag = ~(ECHO | ICANON | IEXTEN | ISIG);
  if(_p.tcsetattr(STDIN_FILENO, TCSANOW, &raw_settings)) {
    throw runtime_error(strerror(errno));
  }
  
  if(_p.ioctl(STDIN_FILENO, KDSKBMODE, K_RAW)) {
    throw runtime_error(strerror(errno));
  }
  _resetters.push_back(boost::bind(&Terminal::reset_raw_kb_mode, this));
}

void Terminal::reset_raw_kb_mode() {
  if(_p.ioctl(STDIN_FILENO, KDSKBMODE, K_XLATE)) {
    throw runtime_error(strerror(errno));
  }
}

void Terminal::reset() {
  if(_resetters.size()) {
    BOOST_REVERSE_FOREACH(boost::function<void () > f, _resetters) {
      f();
    }
    _resetters.clear();
    _p.tcsetattr(STDIN_FILENO, TCSANOW, &_terminal_settings);
  }
}


Terminal::~Terminal() {
  reset();
}


int Terminal::read_character() {
  // struct timeval timeout = {0, 0};
  // fd_set rdfs;
  // FD_ZERO(&rdfs);
  // FD_SET(STDIN_FILENO, &rdfs);  
  // select(1, &rdfs, NULL, NULL, &timeout);
  // if(FD_ISSET(STDIN_FILENO, &rdfs)) {
  //   char buf[1];
  //   read(STDIN_FILENO, (void*)buf, sizeof(buf));
  //   return (buf[0] & 0x7f);
  // }
  // return -1;
}

void Terminal::reset_terminal(int s) {
  Terminal::s_signal_handler_terminal->reset();
}

void Terminal::install_signal_handler() {
  s_signal_handler_terminal = this;
  _p.signal(SIGTERM, reset_terminal);
}
