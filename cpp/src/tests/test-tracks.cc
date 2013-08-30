#include <iostream>
#include <boost/filesystem.hpp>

#include <gmock/gmock.h>

#include "json/json.h"
#include "common/common.hh"

// include mocks and deps
#include "tests/test-openvg-adapter.hh"
#include "assets/assets.hh"

// include objects under test
#include "world/track.hh"


using ::testing::_;
using ::testing::Eq;
using ::testing::IsNull;
using ::testing::An;
using ::testing::Return;
using ::testing::TypedEq;

using namespace std;
using namespace rracer;

namespace fs = boost::filesystem;

class TrackTests : public ::testing::Test {
public:
  TestOpenvgAdaptper* ovg_adapter;
  virtual void SetUp() {
    ovg_adapter = new TestOpenvgAdaptper();
  }

  virtual void TearDown() {
    delete ovg_adapter;
  }

};  


TEST_F(TrackTests, TestSimpleTrackLoading) {
  fs::path json_path("resources/tests/simple-test-track.json");
  ASSERT_TRUE(fs::exists(json_path));
  AssetManager am(*ovg_adapter);
  Track test_track(am, json_path);
  ASSERT_EQ(2, test_track.count());
  const TrackTile& tile_one = test_track[0];
  const TrackTile& tile_two = test_track[1];
  Vector d = tile_one.end().point - tile_one.start().point;
  ASSERT_FLOAT_EQ(20.0, d.norm());
  ASSERT_FLOAT_EQ(tile_one.end().point[0], tile_two.start().point[0]);
  ASSERT_FLOAT_EQ(tile_one.end().point[1], tile_two.start().point[1]);

  ASSERT_FLOAT_EQ(40.0, tile_two.end().point[0]);
  ASSERT_FLOAT_EQ(20.0, tile_two.end().point[1]);
  ASSERT_FLOAT_EQ(90, tile_two.end().direction);

}


