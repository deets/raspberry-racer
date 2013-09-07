#include <boost/foreach.hpp>
#include <math.h>
#include "json/json.h"
#include "world/track.hh"

namespace rracer {

/*		     */
/* Q1   A1   Q0	     */
/*      |	     */
/*      |	     */
/* A2---+ ---A0	     */
/*      |	     */
/*      |	     */
/* Q2   A3   Q3	     */
/*		     */


  TileInfo::TileInfo(const Json::Value tile_info) {
    assert(tile_info.isMember("width") && tile_info["width"].isDouble());
    assert(tile_info.isMember("lanes") && tile_info["lanes"].isArray());
    _width = tile_info["width"].asDouble();
    Json::Value lanes = tile_info["lanes"];
    assert(lanes.size() > 0);
    for(int i = 0; i < lanes.size(); ++i) {
      Json::Value lane = lanes[i];
      assert(lane.isMember("center-offset") && lane["center-offset"].isDouble());
      _lane_offsets.push_back(lane["center-offset"].asDouble());
    }

  }

  TrackTile::TrackTile(const TileInfo& ti)
    : _ti(ti)
  {
  }


  ConnectionPoint TrackTile::start() const {
      return _start;
  }

  ConnectionPoint TrackTile::end() const {
      return _end;
  }

  const Rect TrackTile::bounds() const {
    return _bounds;
  }

  Track::Track(AssetManager& am, fs::path data)
    : _am(am)
  {
    Json::Value json = am.json(data);
    assert(json.isMember("type") && json["type"].asString() == "simple");
    assert(json.isMember("name") && json["name"].isString());
    assert(json.isMember("tile-info") && json["tile-info"].isObject());
    assert(json.isMember("tiles") && json["tiles"].isArray());

    _name = json["name"].asString();

    _tile_info = shared_ptr<TileInfo>(new TileInfo(json["tile-info"]));

    ConnectionPoint connection_point;
    connection_point.point << 0.0, 0.0;
    connection_point.direction = 0.0;

    Rect bounds;
    for(int i=0; i < json["tiles"].size(); ++i) {
      Json::Value tile_json = json["tiles"][i];
      assert(tile_json.isObject());
      shared_ptr<TrackTile> tile = TrackTile::create_tile(tile_json, connection_point, *_tile_info);
      connection_point = tile->end();
      bounds = bounds | tile->bounds();
      _tiles.push_back(tile);
    }
    _bounds = bounds;
    assert(!_bounds.empty());
  }

  Track::~Track() {

  }

  const Rect Track::bounds() const {
    return _bounds;
  }

  const string& Track::name() const {
    return _name;
  }


  size_t Track::count() const {
    return _tiles.size();
  }

  TrackTile& Track::operator[](size_t index) const {
    assert(index < _tiles.size());
    return *_tiles[index];
  }


  void Track::render(const OpenVGCompanion& vgc) const {
    const OpenVGAdapter& vg = vgc.vg();
    {
      VGfloat ground_color [] ={.5, .5, .5, 1.0};
      //      PaintSetter(vgc, ground_color, VG_FILL_PATH);
      VGPaint ground_paint = vg.vgCreatePaint();
      vg.vgSetParameteri(ground_paint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
      VGfloat color_components[4] = {.5, .5, .5, 1.0};
      vg.vgSetParameterfv(ground_paint, VG_PAINT_COLOR, 4, color_components);
      vg.vgSetPaint(ground_paint, VG_FILL_PATH|VG_STROKE_PATH);

      VGPath ground_path = vgc.newPath();
      
      BOOST_FOREACH(shared_ptr<TrackTile> tile, _tiles) {
	tile->append_to_ground_path(vgc, ground_path);
      }
      vg.vgDrawPath(ground_path, VG_FILL_PATH);
      vg.vgDestroyPath(ground_path);
    }

    if(false) {
      VGfloat debug_color[] = {1.0, 1.0, 0, 1.0};
      VGPaint debug_paint = vg.vgCreatePaint();
      vg.vgSetParameteri(debug_paint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
      vg.vgSetParameterfv(debug_paint, VG_PAINT_COLOR, 4, debug_color);
      vg.vgSetPaint(debug_paint, VG_FILL_PATH|VG_STROKE_PATH);
      
      vgc.draw_rect(bounds());
      
      vg.vgDestroyPaint(debug_paint);
    }
  }

} // namespace rracer
