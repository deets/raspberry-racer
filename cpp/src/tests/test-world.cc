#include <gmock/gmock.h>

// include mocks
#include "tests/test-window-adapter.hh"
#include "tests/test-openvg-adapter.hh"

// include objects under test
#include "world/world.hh"

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

