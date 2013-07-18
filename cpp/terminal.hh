#ifndef TERMINAL_HH
#define TERMINAL_HH

#include <termios.h>
#include <unistd.h>

#include <vector>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "posix-adapter.hh"

using namespace std;


class Terminal {

private:

  void reset_raw_kb_mode();


public:
  Terminal(PosixAdapter &adapter);
  virtual ~Terminal();

  void reset();
  void install_signal_handler();
  int read_character();
private:
  struct termios _terminal_settings;
  vector< boost::function<void () > > _resetters;
  PosixAdapter &_p;

};

#endif
