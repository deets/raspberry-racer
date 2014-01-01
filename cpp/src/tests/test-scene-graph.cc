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
  TestWindowAdapter window_adapter;
  EXPECT_CALL(window_adapter, start()).Times(1);
  EXPECT_CALL(window_adapter, end()).Times(1);
  EXPECT_CALL(window_adapter, window_dimensions()).WillRepeatedly(Return(make_pair(1920, 1080)));
  SceneGraph world(window_adapter, *ovg_adapter);
  world.start_frame(1.0/30.0);
  world.end_frame();
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


TEST_F(SceneGraphTests, TestSceneNodesFormHierarchyWhenAddingChildrenToThem) {
  TestChild parent;
  TestChild* child = new TestChild();
  ASSERT_TRUE(parent.parent() == NULL);
  ASSERT_TRUE(child->parent() == NULL);
  parent.add_object(child);
  ASSERT_TRUE(child->parent() == &parent);
}
