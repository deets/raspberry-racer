
#include <stdio.h>
#include <linux/kd.h>

#include <boost/filesystem.hpp>

#include <gmock/gmock.h>

// include mocks
#include "tests/test-posix-adapter.hh"
#include "tests/test-window-adapter.hh"
#include "tests/test-openvg-adapter.hh"

// include objects under test
#include "world/world.hh"
#include "assets/assets.hh"
#include "terminal.hh"

using ::testing::InitGoogleTest;
using ::testing::_;
using ::testing::Eq;
using ::testing::IsNull;
using ::testing::An;
using ::testing::Return;
using ::testing::TypedEq;

namespace fs = boost::filesystem;

TEST(TerminalTest, TestNormalSetup) {
  TestPosixAdapter adapter;
  // setup 
  EXPECT_CALL(adapter, tcgetattr(Eq(0), _));
  EXPECT_CALL(adapter, tcsetattr(Eq(0), Eq(0), _)).Times(2);
  EXPECT_CALL(adapter, ioctl(Eq(0), Eq(KDSKBMODE), TypedEq<unsigned long>(K_RAW))).WillOnce(Return(0));
  EXPECT_CALL(adapter, ioctl(Eq(0), Eq(KDKBDREP), An<kbd_repeat*>())).WillRepeatedly(Return(0));
  EXPECT_CALL(adapter, signal(Eq(SIGTERM), Eq(Terminal::reset_terminal))).Times(1);

  // get character
  EXPECT_CALL(adapter, select(Eq(1), An<fd_set*>(), IsNull(), IsNull(), _)).WillOnce(Return(0));

  // teardown
  //  EXPECT_CALL(adapter, ioctl(Eq(0), Eq(KDKBDREP), An<kbd_repeat*>())).WillOnce(Return(0));
  EXPECT_CALL(adapter, ioctl(Eq(0), Eq(KDSKBMODE),  TypedEq<unsigned long>(K_XLATE))).WillOnce(Return(0));

  {
    Terminal t(adapter);
    t.install_signal_handler();
    int c = t.read_character();
    ASSERT_EQ(-1, c);
  }
}


TEST(WorldTests, TestWorldLifeCycle) {
  TestWindowAdaptper window_adapter;
  TestOpenvgAdaptper ovg_adapter;
  vector<Event> events;
  EXPECT_CALL(window_adapter, start()).Times(1);
  EXPECT_CALL(window_adapter, end()).Times(1);
  World world(window_adapter, ovg_adapter);
  world.begin(events);
  world.end();
}




TEST(AssetTests, TestImageManagement) {
  TestOpenvgAdaptper ovg_adapter;

  fs::path image_path("amiga-ball.png");
  ASSERT_TRUE(fs::exists(image_path));
  
  AssetManager am(&ovg_adapter);
  ImageInfo img = am.image(image_path);
  ImageInfo img2 = am.image(image_path);

  ASSERT_EQ(img.image, img2.image);
}

int main(int argc, char** argv) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
