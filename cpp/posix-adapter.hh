#ifndef POSIX_ADAPTER_HH
#define POSIX_ADAPTER_HH

#include <sys/termios.h>
#include <signal.h>
#include <sys/select.h>
#include <unistd.h>
#ifdef __APPLE__

// mock linux headers

#define K_RAW 0
#define KDSKBMODE 0
#define K_XLATE 0

#else
#include <linux/kd.h>
#endif

class PosixAdapter {
public:
  virtual int ioctl(int, unsigned long, unsigned long)=0;
  virtual int ioctl(int, unsigned long, kbd_repeat*)=0;
  virtual int tcgetattr(int, struct termios *)=0;
  virtual int tcsetattr(int, int, const struct termios *)=0;
  virtual sig_t signal(int sig, sig_t func)=0;
  virtual int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)=0;
  virtual ssize_t read(int fd, void *buf, size_t count)=0;


};

#endif
