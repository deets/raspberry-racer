#include <utility>
#include <gmock/gmock.h>
#include <boost/foreach.hpp>
// include mocks
#include "tests/test-window-adapter.hh"
#include "tests/test-openvg-adapter.hh"

// include objects under test
#include "scene/scene-graph.hh"

using ::testing::Return;
using ::testing::Eq;
using ::testing::An;
using ::testing::NiceMock;
using namespace rracer;

class TestChild : public SceneNode {
  
  
public:
  string name;
  int event_count;
  Real elapsed;

  vector<pair<SceneNode*, SceneNode*> > subtree_objects;

  TestChild() 
    : event_count(0)
    , elapsed(.0)
  {
  }


  TestChild(const string &name) 
    : name(name)
    , event_count(0)
    , elapsed(.0)
  {
  }


  virtual void on_object_added(SceneNode* parent, SceneNode* child) {
    subtree_objects.push_back(make_pair(parent, child));
  }


  virtual void process_frame_events(const GameEventVector& events, const TimeInfo& time_info, EventEmitter emit_event) {
    event_count += events.size();
    this->elapsed += time_info.elapsed();
  }

};


class NextFrameEventAdderTestChild : public SceneNode {
  
  
public:
  string name;
  int event_count;
  Real elapsed;

  NextFrameEventAdderTestChild() 
    : event_count(0)
    , elapsed(.0)
  {
  }

  virtual void process_frame_events(const GameEventVector& events, const TimeInfo& time_info, EventEmitter emit_event) {
    event_count += events.size();
    this->elapsed += time_info.elapsed();
    KeyEvent event(true, K_ESC, 53);
    emit_event(GameEvent(time_info, Event(event)));
  }

};



class SceneGraphTests : public ::testing::Test {
public:
  NiceMock<TestWindowAdapter>* window_adapter;
  NiceMock<TestOpenvgAdaptper>* ovg_adapter;

  virtual void SetUp() {
    ovg_adapter = new NiceMock<TestOpenvgAdaptper>();
    window_adapter = new NiceMock<TestWindowAdapter>();
    ON_CALL(*window_adapter, window_dimensions()).WillByDefault(Return(make_pair(1920, 1080)));
  }

  virtual void TearDown() {
    delete ovg_adapter;
    delete window_adapter;
  }

};


TEST_F(SceneGraphTests, TestSceneGraphLifeCycle) {
  GameEventVector events;
  TestWindowAdapter window_adapter;
  EXPECT_CALL(window_adapter, start()).Times(1);
  EXPECT_CALL(window_adapter, end()).Times(1);
  EXPECT_CALL(window_adapter, window_dimensions()).WillRepeatedly(Return(make_pair(1920, 1080)));
  SceneGraph world(window_adapter, *ovg_adapter);
  world.start_frame(events, 1.0/30.0);
  world.end_frame();
}

TEST_F(SceneGraphTests, TestSceneGraphEventDispatchToChildren) {
  GameEventVector events;
  KeyEvent event(false, K_ESC, 1);
  events.push_back(GameEvent(TimeInfo(), event));

  TestChild* parent = new TestChild();
  TestChild* child = new TestChild();
  parent->add_object(child);
  {
    SceneGraph world(*window_adapter, *ovg_adapter);
    world.add_object(parent);
    world.start_frame(events, 1.0/30.0);
    world.end_frame();
  
    ASSERT_TRUE(parent->event_count > 0);
    ASSERT_TRUE(child->event_count > 0);
    ASSERT_FLOAT_EQ(1.0/ 30.0, parent->elapsed);
    ASSERT_FLOAT_EQ(1.0/ 30.0, child->elapsed);
  }
}


TEST_F(SceneGraphTests, TestSceneGraphTraverseObjectTree) {
  SceneGraph world(*window_adapter, *ovg_adapter);
  vector<SceneNode*> expected;
  {
    vector<SceneNode*> result;
    for(SceneGraph::iterator it = world.begin(); it != world.end(); ++it) {
      result.push_back(&*it);
    }
    ASSERT_EQ(expected, result);
  }

  TestChild* parent = new TestChild("parent");
  world.add_object(parent);
  expected.push_back(parent);
  {
    vector<SceneNode*> result;
    for(SceneGraph::iterator it = world.begin(); it != world.end(); ++it) {
      result.push_back(&*it);
    }
    ASSERT_EQ(expected, result);
  }

  TestChild* child = new TestChild("child");
  parent->add_object(child);
  expected.push_back(child);
  {
    vector<SceneNode*> result;
    for(SceneGraph::iterator it = world.begin(); it != world.end(); ++it) {
      result.push_back(&*it);
    }
    ASSERT_EQ(expected, result);
  }

  TestChild* other = new TestChild("other");
  world.add_object(other);
  expected.push_back(other);
  {
    vector<SceneNode*> result;
    for(SceneGraph::iterator it = world.begin(); it != world.end(); ++it) {
      result.push_back(&*it);
    }
    ASSERT_EQ(expected, result);
  }
}


TEST_F(SceneGraphTests, TestSceneGraphRetainsNextFrameEvents) {
  GameEventVector events;
  SceneGraph world(*window_adapter, *ovg_adapter);
  NextFrameEventAdderTestChild* other = new NextFrameEventAdderTestChild();
  world.add_object(other);
  world.start_frame(events, 1.0/30.0);
  world.end_frame();
  ASSERT_EQ(0, other->event_count);
  world.start_frame(events, 1.0/30.0);
  world.end_frame();
  ASSERT_EQ(1, other->event_count);
  world.start_frame(events, 1.0/30.0);
  world.end_frame();
  ASSERT_EQ(2, other->event_count);
}


TEST_F(SceneGraphTests, TestSceneNodesFormHierarchyWhenAddingChildrenToThem) {
  TestChild parent;
  TestChild* child = new TestChild();
  ASSERT_TRUE(parent.parent() == NULL);
  ASSERT_TRUE(child->parent() == NULL);
  parent.add_object(child);
  ASSERT_TRUE(child->parent() == &parent);
}


TEST_F(SceneGraphTests, TestSceneNodesAddingChildrenPropagatesUpwardsParentChain) {
  shared_ptr<TestChild> grandparent = shared_ptr<TestChild>(new TestChild());
  TestChild* parent = new TestChild();
  TestChild* child = new TestChild();
  grandparent->add_object(parent);
  parent->add_object(child);
  ASSERT_TRUE(grandparent.get() == parent->parent());
  ASSERT_TRUE(parent == child->parent());
  ASSERT_EQ(2, grandparent->subtree_objects.size());
  ASSERT_EQ(1, parent->subtree_objects.size());
  ASSERT_EQ(0, child->subtree_objects.size());
}
