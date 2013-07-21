#include <gmock/gmock.h>
#include <stdio.h>

#include "posix-adapter.hh"
#include "terminal.hh"


using ::testing::InitGoogleTest;
using ::testing::_;
using ::testing::Eq;
using ::testing::IsNull;
using ::testing::An;
using ::testing::Return;

class TestPosixAdapter : public PosixAdapter {
public:

  virtual int ioctl(int, unsigned long, ...) {
    return 0;
  }

  MOCK_METHOD2(tcgetattr, int(int fd, struct termios *t));

  MOCK_METHOD3(tcsetattr, int(int, int, const struct termios *));

  MOCK_METHOD2(signal, sig_t (int sig, sig_t func));

  MOCK_METHOD5(select, int (int, fd_set *, fd_set *, fd_set *, struct timeval *));

  MOCK_METHOD3(read, ssize_t(int, void *, size_t));

};


TEST(TerminalTest, TestNormalSetup) {
  TestPosixAdapter adapter;
  EXPECT_CALL(adapter, tcgetattr(Eq(0), _));
  EXPECT_CALL(adapter, tcsetattr(Eq(0), Eq(0), _)).Times(2);
  EXPECT_CALL(adapter, signal(Eq(SIGTERM), Eq(Terminal::reset_terminal))).Times(1);
  EXPECT_CALL(adapter, select(Eq(1), An<fd_set*>(), IsNull(), IsNull(), _)).WillOnce(Return(0));
  {
    Terminal t(adapter);
    t.install_signal_handler();
    int c = t.read_character();
    ASSERT_EQ(-1, c);
  }
}



int main(int argc, char** argv) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
