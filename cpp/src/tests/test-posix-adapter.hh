#ifndef TEST_POSIX_ADAPTER_HH
#define TEST_POSIX_ADAPTER_HH

#include <gmock/gmock.h>

#include "rpi/posix-adapter.hh"

class TestPosixAdapter : public PosixAdapter {
public:

  MOCK_CONST_METHOD3(ioctl, int(int, unsigned long, unsigned long));

  MOCK_CONST_METHOD3(ioctl, int(int, unsigned long, kbd_repeat *));

  MOCK_CONST_METHOD2(tcgetattr, int(int fd, struct termios *t));

  MOCK_CONST_METHOD3(tcsetattr, int(int, int, const struct termios *));

  MOCK_CONST_METHOD2(signal, sig_t (int sig, sig_t func));

  MOCK_CONST_METHOD5(select, int (int, fd_set *, fd_set *, fd_set *, struct timeval *));

  MOCK_CONST_METHOD3(read, ssize_t(int, void *, size_t));

};

#endif
