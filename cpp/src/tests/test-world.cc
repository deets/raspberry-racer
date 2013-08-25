#include <gmock/gmock.h>

// include mocks
#include "tests/test-window-adapter.hh"
#include "tests/test-openvg-adapter.hh"

// include objects under test
#include "world/world.hh"

TEST(WorldTests, TestWorldLifeCycle) {
  TestWindowAdaptper window_adapter;
  TestOpenvgAdaptper ovg_adapter;
  InputEventVector events;
  EXPECT_CALL(window_adapter, start()).Times(1);
  EXPECT_CALL(window_adapter, end()).Times(1);
  World world(window_adapter, ovg_adapter);
  world.begin(events, 1.0/30.0);
  world.end();
}

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

TEST(WorldTests, TestWorldEventDispatchToChildren) {
  TestWindowAdaptper window_adapter;
  TestOpenvgAdaptper ovg_adapter;
  InputEventVector events;
  InputEvent event = { false, K_ESC, 1 };
  events.push_back(event);

  EXPECT_CALL(window_adapter, start()).Times(1);
  EXPECT_CALL(window_adapter, end()).Times(1);

  TestChild* parent = new TestChild();
  TestChild* child = new TestChild();
  parent->add_object(child);
  {
    World world(window_adapter, ovg_adapter);
    world.add_object(parent);
    world.begin(events, 1.0/30.0);
    world.end();
  
    ASSERT_TRUE(parent->event_count > 0);
    ASSERT_TRUE(child->event_count > 0);
    ASSERT_FLOAT_EQ(1.0/ 30.0, parent->elapsed);
    ASSERT_FLOAT_EQ(1.0/ 30.0, child->elapsed);
  }
}

