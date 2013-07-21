#include <sys/ioctl.h>
#include <sys/termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <bcm_host.h>

#include "rpi-adapter-impl.hh"


int RPiAdapterImpl::ioctl(int fd, unsigned long p1, unsigned long p2) {
  return ::ioctl(fd, p1, p2);
}

int RPiAdapterImpl::ioctl(int fd, unsigned long mode, kbd_repeat *kbdrep_s) {
  return ::ioctl(fd, mode, kbdrep_s);
}


int RPiAdapterImpl::tcgetattr(int fd, struct termios *t) {
  return ::tcgetattr(fd, t);
}


int RPiAdapterImpl::tcsetattr(int fd, int mode, const struct termios * t) {
  return ::tcsetattr(fd, mode, t);
}


sig_t RPiAdapterImpl::signal(int sig, sig_t func) {
  return ::signal(sig, func);
}


int RPiAdapterImpl::select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) {
  return ::select(nfds, readfds, writefds, exceptfds, timeout);
}


ssize_t RPiAdapterImpl::read(int fd, void *buf, size_t count) {
  return ::read(fd, buf, count);
}


void RPiAdapterImpl::bcm_host_init() {
  ::bcm_host_init();
}
void RPiAdapterImpl::bcm_host_deinit() {
  ::bcm_host_deinit();
}


int32_t RPiAdapterImpl::graphics_get_display_size(
      const uint16_t display_number,
      uint32_t *width,
      uint32_t *height) {
  return ::graphics_get_display_size(display_number, width, height);
}


RPiAdapterImpl::~RPiAdapterImpl() {
}
