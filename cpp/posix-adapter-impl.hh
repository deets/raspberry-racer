#ifndef POSIX_ADAPTER_IMPL_HH
#define POSIX_ADAPTER_IMPL_HH

#include "posix-adapter.hh"

class PosixAdapterImpl : public PosixAdapter {

public:
  virtual ~PosixAdapterImpl();

  virtual int ioctl(int, unsigned long, unsigned long);
  virtual int ioctl(int, unsigned long, kbd_repeat*);
  virtual int tcgetattr(int, struct termios *);
  virtual int tcsetattr(int, int, const struct termios *);
  virtual sig_t signal(int sig, sig_t func);
  virtual int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
  virtual ssize_t read(int fd, void *buf, size_t count);

};

#endif
