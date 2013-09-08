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

// include objects under test
#include "world/world.hh"
#include "world/car.hh"

using ::testing::_;
using ::testing::Eq;
using ::testing::IsNull;
using ::testing::An;
using ::testing::Return;
using ::testing::TypedEq;

using namespace std;
using namespace rracer;

namespace fs = boost::filesystem;

class CarTests : public ::testing::Test {
public:
  TestOpenvgAdaptper* ovg_adapter;
  AssetManager* asset_manager;
  TestWindowAdapter* window_adapter;

  virtual void SetUp() {
    window_adapter = new TestWindowAdapter();
    ovg_adapter = new TestOpenvgAdaptper();
    fs::path cars_path("resources/cars");
    asset_manager = new AssetManager(*ovg_adapter, cars_path);
  }

  virtual void TearDown() {
    delete window_adapter;
    delete asset_manager;
    delete ovg_adapter;
  }

};  


TEST_F(CarTests, TestCarLoading) {
  JH car_info = jh("image", "yellow.png")("length", 12.0)("slot-offset", 3.0);
  Car car(*asset_manager, car_info);
  World* world = new World(*window_adapter, *ovg_adapter);
}
