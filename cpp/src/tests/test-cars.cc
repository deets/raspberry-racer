#include <iostream>
#include <boost/filesystem.hpp>

#include <gmock/gmock.h>

#include "json/json.h"
#include "common/common.hh"

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

  virtual void SetUp() {
    window_adapter = new NiceMock<TestWindowAdapter>();
    ovg_adapter = new NiceMock<TestOpenvgAdaptper>();
    fs::path cars_path("resources/cars");
    asset_manager = new AssetManager(*ovg_adapter, cars_path);
    EXPECT_CALL(*window_adapter, window_dimensions()).WillRepeatedly(Return(make_pair(1920, 1080)));

  }

  virtual void TearDown() {
    delete window_adapter;
    delete asset_manager;
    delete ovg_adapter;
  }

};  


TEST_F(CarTests, TestCarLoading) {
  JH car_info = jh
    ("image", "yellow.png")
    ("length", 12.0)
    ("width", 1.0)
    ("mass", 100)
    ("slot-offset", 3.0)
    ;
  InputEventVector events;
  World world(*window_adapter, *ovg_adapter);
  {
    Car* car = new Car(*asset_manager, car_info);
    world.add_object(car);
    world.begin(events, 1/30.0);
  }
}
