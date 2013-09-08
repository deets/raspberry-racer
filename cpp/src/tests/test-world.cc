#include <utility>
#include <gmock/gmock.h>

// include mocks
#include "tests/test-window-adapter.hh"
#include "tests/test-openvg-adapter.hh"

// include objects under test
#include "world/world.hh"

using ::testing::Return;
using ::testing::Eq;
using ::testing::An;
using ::testing::NiceMock;
using namespace rracer;

class TestChild : public WorldObject {
  
  
public:
  int event_count;
  double elapsed;

  TestChild() 
    : event_count(0)
    , elapsed(.0)
  {
  }

  virtual void process_input_events(const InputEventVector& events, double elapsed) {
    event_count += events.size();
    this->elapsed += elapsed;
  }

};


class WorldTests : public ::testing::Test {
public:
  TestWindowAdapter* window_adapter;
  NiceMock<TestOpenvgAdaptper>* ovg_adapter;

  virtual void SetUp() {
    ovg_adapter = new NiceMock<TestOpenvgAdaptper>();
    window_adapter = new TestWindowAdapter();
    EXPECT_CALL(*window_adapter, start()).Times(1);
    EXPECT_CALL(*window_adapter, end()).Times(1);
    EXPECT_CALL(*window_adapter, window_dimensions()).WillRepeatedly(Return(make_pair(1920, 1080)));
  }

  virtual void TearDown() {
    delete ovg_adapter;
    delete window_adapter;
  }

};


TEST_F(WorldTests, TestWorldLifeCycle) {
  InputEventVector events;
  World world(*window_adapter, *ovg_adapter);
  world.begin(events, 1.0/30.0);
  world.end();
}

TEST_F(WorldTests, TestWorldEventDispatchToChildren) {
  InputEventVector events;
  InputEvent event = { false, K_ESC, 1 };
  events.push_back(event);

  TestChild* parent = new TestChild();
  TestChild* child = new TestChild();
  parent->add_object(child);
  {
    World world(*window_adapter, *ovg_adapter);
    world.add_object(parent);
    world.begin(events, 1.0/30.0);
    world.end();
  
    ASSERT_TRUE(parent->event_count > 0);
    ASSERT_TRUE(child->event_count > 0);
    ASSERT_FLOAT_EQ(1.0/ 30.0, parent->elapsed);
    ASSERT_FLOAT_EQ(1.0/ 30.0, child->elapsed);
  }
}

