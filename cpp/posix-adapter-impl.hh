#ifndef POSIX_ADAPTER_IMPL_HH
#define POSIX_ADAPTER_IMPL_HH

#include "posix-adapter.hh"

class PosixAdapterImpl : public PosixAdapter {

public:
  virtual int ioctl(int, unsigned long, ...);
  virtual int tcgetattr(int, struct termios *);
  virtual int tcsetattr(int, int, const struct termios *);
  virtual sig_t signal(int sig, sig_t func);
  virtual ~PosixAdapterImpl();

};

#endif
