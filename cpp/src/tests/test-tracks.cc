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
  ASSERT_EQ(8, test_track.count());
  for(int i = 0; i < 8; ++i) {
    const TrackTile& tile_one = test_track[i];
    const TrackTile& tile_two = test_track[(i+1) % 8];
    ASSERT_NEAR(tile_one.end().point[0], tile_two.start().point[0], 0.000001);
    ASSERT_NEAR(tile_one.end().point[1], tile_two.start().point[1], 0.000001);
  }
  ASSERT_FALSE(test_track.bounds().empty());
  
}


