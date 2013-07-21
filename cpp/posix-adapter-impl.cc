#include "posix-adapter-impl.hh"


int PosixAdapterImpl::ioctl(int fd, unsigned long param, ...) {
  return ioctl(fd, param);
}


int PosixAdapterImpl::tcgetattr(int fd, struct termios *t) {
  return tcgetattr(fd, t);
}


int PosixAdapterImpl::tcsetattr(int fd, int mode, const struct termios * t) {
  return tcsetattr(fd, mode, t);
}


sig_t PosixAdapterImpl::signal(int sig, sig_t func) {
  return signal(sig, func);
}


PosixAdapterImpl::~PosixAdapterImpl() {
}
