#include "rpi/posix-adapter.hh"


class PosixAdapterImpl : public PosixAdapter {

public:
  virtual int ioctl(int, unsigned long, unsigned long) const;
  virtual int ioctl(int, unsigned long, kbd_repeat*) const;
  virtual int tcgetattr(int, struct termios *) const;
  virtual int tcsetattr(int, int, const struct termios *) const;
  virtual sig_t signal(int sig, sig_t func) const;
  virtual int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) const;
  virtual ssize_t read(int fd, void *buf, size_t count) const;

};
