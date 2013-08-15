#ifndef TERMINAL_HH
#define TERMINAL_HH

#include <termios.h>
#include <unistd.h>

#include <utility>
#include <vector>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "posix-adapter.hh"
#include "common/non-copyable.hh"

using namespace std;


class Terminal: public NonCopyAble {

private:

  void reset_raw_kb_mode();
  void reset_kb_repeat();

  // prevent copying
  Terminal(const Terminal&);
  Terminal & operator=(const Terminal&);

public:
  Terminal(PosixAdapter &adapter);
  virtual ~Terminal();

  void reset();
  void install_signal_handler();
  int read_character();

  static void reset_terminal(int signal);

private:
  struct termios _terminal_settings;
  vector< boost::function<void () > > _resetters;
  PosixAdapter &_p;

  static Terminal *s_signal_handler_terminal;

};

#endif
