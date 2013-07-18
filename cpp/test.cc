#include <gtest/gtest.h>
#include <stdio.h>

#include "posix-adapter.hh"
#include "terminal.hh"


using ::testing::InitGoogleTest;

class TestPosixAdapter : public PosixAdapter {
public:

  virtual int ioctl(int, unsigned long, ...) {
    return 0;
  }

  virtual int tcgetattr(int fd, struct termios *t) {
    return 0;
  }

  virtual int tcsetattr(int, int, const struct termios *) {
    return 0;
  }

  virtual sig_t signal(int sig, sig_t func) {
    return NULL;
  }

};


TEST(TerminalTest, TestNormalSetup) {
  TestPosixAdapter adapter;
  Terminal t(adapter);
}



int main(int argc, char** argv) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
