#ifndef TERMINAL_HH
#define TERMINAL_HH

#include <termios.h>
#include <unistd.h>

#include <vector>
#include <boost/bind.hpp>
#include <tr1/functional>

using namespace std;


class Terminal {

private:

  struct termios _terminal_settings;

  vector< std::tr1::function<void (Terminal*) > > _resetters;

  void reset_raw_kb_mode();

public:
  Terminal();
  virtual ~Terminal();

  void reset();
  void install_signal_handler();
  int read_character();
};

#endif
