#include <utility>
#include <gmock/gmock.h>
#include <boost/foreach.hpp>
// include mocks

// include objects under test
#include "game/game.hh"

using ::testing::Return;
using ::testing::Eq;
using ::testing::An;
using ::testing::NiceMock;
using namespace rracer;

class TestGameObject : public GameObject {
  
  
public:
  string name;
  int event_count;
  Real elapsed;

  vector<pair<GameObject*, GameObject*> > subtree_objects;

  TestGameObject() 
    : GameObject()
    , name("<unknown>")
    , event_count(0)
    , elapsed(.0)
  {
  }


  TestGameObject(const string &name) 
    : GameObject()
    , name(name)
    , event_count(0)
    , elapsed(.0)
  {
  }


  virtual void on_object_added(GameObject* parent, GameObject* child) {
    subtree_objects.push_back(make_pair(parent, child));
  }


  virtual void process_frame_events(const GameEventVector& events, const TimeInfo& time_info, EventEmitter emit_event) {
    event_count += events.size();
    this->elapsed += time_info.elapsed();
  }

};


class NextFrameEventAdderTestGameObject : public GameObject {
  
  
public:
  string name;
  int event_count;
  Real elapsed;

  NextFrameEventAdderTestGameObject() 
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



class GameTests : public ::testing::Test {
public:

  virtual void SetUp() {
  }

  virtual void TearDown() {
  }

};



TEST_F(GameTests, TestGameEventDispatchToChildren) {
  GameEventVector events;
  KeyEvent event(false, K_ESC, 1);
  events.push_back(GameEvent(TimeInfo(), event));

  TestGameObject* parent = new TestGameObject();
  TestGameObject* child = new TestGameObject();
  parent->add_object(child);
  {
    Game world(NULL);
    world.add_object(parent);
    world.step(events, 1.0/30.0);
  
    ASSERT_TRUE(parent->event_count > 0);
    ASSERT_TRUE(child->event_count > 0);
    ASSERT_FLOAT_EQ(1.0/ 30.0, parent->elapsed);
    ASSERT_FLOAT_EQ(1.0/ 30.0, child->elapsed);
  }
}


TEST_F(GameTests, TestGameTraverseObjectTree) {
  Game world(NULL);
  vector<GameObject*> expected;
  {
    vector<GameObject*> result;
    for(Game::iterator it = world.begin(); it != world.end(); ++it) {
      result.push_back(&*it);
    }
    ASSERT_EQ(expected, result);
  }

  TestGameObject* parent = new TestGameObject("parent");
  world.add_object(parent);
  expected.push_back(parent);
  {
    vector<GameObject*> result;
    for(Game::iterator it = world.begin(); it != world.end(); ++it) {
      result.push_back(&*it);
    }
    ASSERT_EQ(expected, result);
  }

  TestGameObject* child = new TestGameObject("child");
  parent->add_object(child);
  expected.push_back(child);
  {
    vector<GameObject*> result;
    for(Game::iterator it = world.begin(); it != world.end(); ++it) {
      result.push_back(&*it);
    }
    ASSERT_EQ(expected, result);
  }

  TestGameObject* other = new TestGameObject("other");
  world.add_object(other);
  expected.push_back(other);
  {
    vector<GameObject*> result;
    for(Game::iterator it = world.begin(); it != world.end(); ++it) {
      result.push_back(&*it);
    }
    ASSERT_EQ(expected, result);
  }
}


TEST_F(GameTests, TestGameRetainsNextFrameEvents) {
  GameEventVector events;
  Game world(NULL);
  NextFrameEventAdderTestGameObject* other = new NextFrameEventAdderTestGameObject();
  world.add_object(other);
  world.step(events, 1.0/30.0);
  ASSERT_EQ(0, other->event_count);
  world.step(events, 1.0/30.0);
  ASSERT_EQ(1, other->event_count);
  world.step(events, 1.0/30.0);
  ASSERT_EQ(2, other->event_count);
}


TEST_F(GameTests, TestGameObjectsFormHierarchyWhenAddingChildrenToThem) {
  TestGameObject parent;
  TestGameObject* child = new TestGameObject();
  ASSERT_TRUE(parent.parent() == NULL);
  ASSERT_TRUE(child->parent() == NULL);
  parent.add_object(child);
  ASSERT_TRUE(child->parent() == &parent);
}


TEST_F(GameTests, TestGameObjectsAddingChildrenPropagatesUpwardsParentChain) {
  shared_ptr<TestGameObject> grandparent = shared_ptr<TestGameObject>(new TestGameObject());
  TestGameObject* parent = new TestGameObject();
  TestGameObject* child = new TestGameObject();
  grandparent->add_object(parent);
  parent->add_object(child);
  ASSERT_TRUE(grandparent.get() == parent->parent());
  ASSERT_TRUE(parent == child->parent());
  ASSERT_EQ(2, grandparent->subtree_objects.size());
  ASSERT_EQ(1, parent->subtree_objects.size());
  ASSERT_EQ(0, child->subtree_objects.size());
}
