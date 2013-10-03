#include <iostream>
#include <boost/filesystem.hpp>

#include <gmock/gmock.h>

#include "json/json.h"
#include "common/common.hh"
#include "tests/common.hh"

// include mocks and deps
#include "tests/json-helper.hh"
#include "tests/test-openvg-adapter.hh"
#include "assets/assets.hh"

// include objects under test
#include "world/track.hh"
#include "world/track-tiles.hh"

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
    ASSERT_VECTOR_EQ(tile_one.end().point, tile_two.start().point);
  }
  ASSERT_FALSE(test_track.bounds().empty());
  
}


TEST_F(TrackTests, TestTilePositions) {
  JH ti_json = jh("width", 20.0)("slot-width", 0.4)("lanes", jh(jh("center-offset", 7.5))(jh("center-offset", -7.5)));
  TileInfo ti(ti_json);
  {
    ConnectionPoint start;
    start.point = Vector(0, 0);
    start.direction = 0;
    JH straight_json = jh("type", "startinggrid")("length", 20.0);
    Straight straight(straight_json, start, ti);
    ASSERT_EQ(start.point + Vector(0, 7.5), straight.position(0, 0));
    ASSERT_EQ(start.point + Vector(0, -7.5), straight.position(0, 1));
    ASSERT_VECTOR_EQ((start.point + Vector(0, 7.5) + Vector(20, 0)), straight.position(1.0, 0));
  }
  {
    ConnectionPoint start;
    start.point = Vector(0, 0);
    start.direction = 90;
    JH straight_json = jh("type", "startinggrid")("length", 20.0);
    Straight straight(straight_json, start, ti);
    ASSERT_VECTOR_EQ(start.point + Vector(-7.5, 0), straight.position(0, 0));
    ASSERT_VECTOR_EQ(start.point + Vector(7.5, 0), straight.position(0, 1));
    ASSERT_VECTOR_EQ((start.point + Vector(-7.5, 0) + Vector(0, 20)), straight.position(1.0, 0));
  }

  {
    ConnectionPoint start;
    start.point = Vector(0, 0);
    start.direction = 0;
    JH curve_json = jh("type", "curve")("radius", 10.0)("degrees", 90.0);
    Curve curve(curve_json, start, ti);
    Vector p1 = curve.position(0, 0);
    Vector p2 = curve.position(0, 1);
    ASSERT_VECTOR_EQ(start.point + Vector(0, 7.5), p1);
    ASSERT_VECTOR_EQ(start.point + Vector(0, -7.5), p2);
    p1 = curve.position(1.0, 0);
    p2 = curve.position(1.0, 1);
    ASSERT_VECTOR_EQ(curve.end().point + Vector(-7.5, 0), p1);
    ASSERT_VECTOR_EQ(curve.end().point + Vector(7.5, 0), p2);
  }

}


TEST_F(TrackTests, TestNearestPoint) {
  JH ti_json = jh("width", 20.0)("slot-width", 0.4)("lanes", jh(jh("center-offset", 7.5))(jh("center-offset", -7.5)));
  TileInfo ti(ti_json);
  {
    ConnectionPoint start;
    start.point = Vector(0, 0);
    start.direction = 0;
    JH straight_json = jh("type", "startinggrid")("length", 20.0);
    Straight straight(straight_json, start, ti);
    NearestPointInfo pi = straight.nearest_point(0, Vector(10.0, 8.5));
    ASSERT_VECTOR_EQ(Vector(10.0, 7.5), pi.point);
    ASSERT_EQ(0, pi.lane);
    ASSERT_FLOAT_EQ(.5, pi.offset);
    ASSERT_FLOAT_EQ(1.0, pi.distance);

    pi = straight.nearest_point(1, Vector(10.0, -8.5));
    ASSERT_VECTOR_EQ(Vector(10.0, -7.5), pi.point);
    ASSERT_EQ(1, pi.lane);
    ASSERT_FLOAT_EQ(.5, pi.offset);
    ASSERT_FLOAT_EQ(1.0, pi.distance);
  }

  {
    ConnectionPoint start;
    start.point = Vector(0, 0);
    start.direction = 0;
    JH curve_json = jh("type", "curve")("radius", 10.0)("degrees", 90.0);
    Curve curve(curve_json, start, ti);
    Vector cp(0, 20);
    Vector swipe(0, -20 + 8);
    const Vector rp = rotation(45) * swipe;
    Vector np = rp + cp;
    NearestPointInfo pi = curve.nearest_point(0, np);
    //ASSERT_VECTOR_EQ(lane_point, pi.point);
    ASSERT_EQ(0, pi.lane);
    ASSERT_FLOAT_EQ(.5, pi.offset);
    ASSERT_FLOAT_EQ(.5, pi.distance);
  }

}

TEST_F(TrackTests, TestStartingPosition) {
  fs::path json_path("resources/tests/simple-test-track.json");
  ASSERT_TRUE(fs::exists(json_path));
  AssetManager am(*ovg_adapter);
  Track test_track(am, json_path);
  ConnectionPoint p1 = test_track.starting_position(0, 0);
  
}
