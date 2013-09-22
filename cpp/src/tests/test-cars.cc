#include <iostream>
#include <boost/filesystem.hpp>

#include <gmock/gmock.h>

#include "json/json.h"
#include "common/common.hh"
#include "tests/common.hh"

// include mocks and deps
#include "tests/json-helper.hh"
#include "tests/test-openvg-adapter.hh"
#include "tests/test-window-adapter.hh"
#include "assets/assets.hh"
#include "events/events.hh"

// include objects under test
#include "world/world.hh"
#include "world/car.hh"

using ::testing::_;
using ::testing::Eq;
using ::testing::IsNull;
using ::testing::An;
using ::testing::Return;
using ::testing::TypedEq;
using ::testing::NiceMock;

using namespace std;
using namespace rracer;

namespace fs = boost::filesystem;

class CarTests : public ::testing::Test {
public:
  NiceMock<TestOpenvgAdaptper>* ovg_adapter;
  AssetManager* asset_manager;
  NiceMock<TestWindowAdapter>* window_adapter;
  JH car_info;

  virtual void SetUp() {
    window_adapter = new NiceMock<TestWindowAdapter>();
    ovg_adapter = new NiceMock<TestOpenvgAdaptper>();
    fs::path cars_path("resources/cars");
    asset_manager = new AssetManager(*ovg_adapter, cars_path);
    EXPECT_CALL(*window_adapter, window_dimensions()).WillRepeatedly(Return(make_pair(1920, 1080)));
    car_info = jh
      ("image", "yellow.png")
      ("length", 12.0)
      ("width", 8.0)
      ("mass", 100)
      ("slot-offset", 3.0)
      ("power", 1500.0)
      ("wheels", 
       jh
       (jh("offset", jh(-5.0)(2.0))("width", 2.0)("diameter", 3.0)("mass", 15))
       (jh("offset", jh(-5.0)(-2.0))("width", 2.0)("diameter", 3.0)("mass", 15))
      )
      ;
  }

  virtual void TearDown() {
    delete window_adapter;
    delete asset_manager;
    delete ovg_adapter;
  }

};  


class TestCar : public Car {

public:
  TestCar(AssetManager& am, const Json::Value& def) 
    : Car(am, def) 
  {
  }
  int wheel_count() const { return _wheels.size(); }
  int destroyer_count() const { return _destroyers.size(); }
};


TEST_F(CarTests, TestCarLoading) {
  InputEventVector events;
  // simulate the user accelerating
  InputEvent event = { true, K_UP, 126 };
  events.push_back(event);
  World world(*window_adapter, *ovg_adapter);
  {
    TestCar* car = new TestCar(*asset_manager, car_info);
    world.add_object(car);
    ASSERT_EQ(2, car->wheel_count());
    ASSERT_EQ(3, car->destroyer_count());
    ConnectionPoint pos = car->position();
    ASSERT_FLOAT_EQ(0, pos.direction);
    ASSERT_VECTOR_EQ(Vector(3.0, .0), pos.point);
    world.begin(events, 1/30.0);
  }
}


TEST_F(CarTests, TestCarPlacing) {
  InputEventVector events;
  World world(*window_adapter, *ovg_adapter);
  {
    TestCar* car = new TestCar(*asset_manager, car_info);
    world.add_object(car);
    ConnectionPoint dest = { Vector(10, 20), 45 };
    car->place(dest);
    ConnectionPoint pos = car->position();
    ASSERT_FLOAT_EQ(dest.direction, pos.direction);
    ASSERT_VECTOR_EQ(dest.point, pos.point);
  }
}






