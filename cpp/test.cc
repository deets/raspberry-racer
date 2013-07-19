#include <gmock/gmock.h>
#include <stdio.h>

#include "posix-adapter.hh"
#include "terminal.hh"


using ::testing::InitGoogleTest;
using ::testing::_;
using ::testing::Eq;


class TestPosixAdapter : public PosixAdapter {
public:

  virtual int ioctl(int, unsigned long, ...) {
    return 0;
  }

  MOCK_METHOD2(tcgetattr, int(int fd, struct termios *t));

  MOCK_METHOD3(tcsetattr, int(int, int, const struct termios *));

  MOCK_METHOD2(signal, sig_t (int sig, sig_t func));

};


TEST(TerminalTest, TestNormalSetup) {
  TestPosixAdapter adapter;
  EXPECT_CALL(adapter, tcgetattr(Eq(0), _));
  EXPECT_CALL(adapter, tcsetattr(Eq(0), Eq(0), _)).Times(2);
  {
    Terminal t(adapter);
  }
}



int main(int argc, char** argv) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
