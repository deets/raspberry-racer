#include <sys/ioctl.h>
#include <sys/termios.h>
#include <unistd.h>
#include <sys/select.h>

#include "posix-adapter-impl.hh"


int PosixAdapterImpl::ioctl(int fd, unsigned long p1, unsigned long p2) const {
  return ::ioctl(fd, p1, p2);
}

int PosixAdapterImpl::ioctl(int fd, unsigned long mode, kbd_repeat *kbdrep_s) const {
  return ::ioctl(fd, mode, kbdrep_s);
}


int PosixAdapterImpl::tcgetattr(int fd, struct termios *t) const {
  return ::tcgetattr(fd, t);
}


int PosixAdapterImpl::tcsetattr(int fd, int mode, const struct termios * t) const {
  return ::tcsetattr(fd, mode, t);
}


sig_t PosixAdapterImpl::signal(int sig, sig_t func) const {
  return ::signal(sig, func);
}


int PosixAdapterImpl::select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) const {
  return ::select(nfds, readfds, writefds, exceptfds, timeout);
}


ssize_t PosixAdapterImpl::read(int fd, void *buf, size_t count) const {
  return ::read(fd, buf, count);
}
