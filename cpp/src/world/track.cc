#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>

#include <math.h>
#include "json/json.h"
#include "world/track.hh"

using namespace boost::lambda;

namespace rracer {

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


  class Straight : public TrackTile {

  public:
    Straight(const Json::Value& tile, const ConnectionPoint& start, const TileInfo& ti) 
      : TrackTile(ti)
    {
      assert(tile.isMember("length") && tile["length"].isDouble());
      Real length = tile["length"].asDouble();
      AffineTransform t = rotation(start.direction);
      Vector l;
      l << length, 0;
      l = t * l;
      _start = start;
      _end.point = _start.point + l;
      _end.direction = _start.direction;

      struct Local {
	static bool comp_x(const Vector& a, const Vector& b) { return a[0] < b[0]; }
	static bool comp_y(const Vector& a, const Vector& b) { return a[1] < b[1]; }
      };
      
      const vector<Vector> points = corners();
      Real left = (*min_element(points.begin(), points.end(), &Local::comp_x))[0];
      Real right = (*max_element(points.begin(), points.end(), &Local::comp_x))[0];
      Real bottom = (*min_element(points.begin(), points.end(), &Local::comp_y))[1];
      Real top = (*max_element(points.begin(), points.end(), &Local::comp_y))[1];
      _bounds = Rect::from_corners(Vector(left, bottom), Vector(right, top));
    }

    const vector<Vector> corners() const {
      vector<Vector> res(4);
      Real whalf = _ti.width() / 2.0;
      Vector offset(0, whalf);
      AffineTransform t = rotation(start().direction);
      offset = t * offset;
      const Vector start = this->start().point;
      const Vector end =  this->end().point;
      res.push_back(start + offset);
      res.push_back(end + offset);
      res.push_back(end - offset);
      res.push_back(start - offset);
      return res;
    }

    virtual void append_to_ground_path(const OpenVGCompanion& vgc, VGPath ground_path) const {
      const vector<Vector> points = corners();
      vgc.move_to(ground_path, points[0], VG_ABSOLUTE);
      vgc.line_to(ground_path, points[1], VG_ABSOLUTE);
      vgc.line_to(ground_path, points[2], VG_ABSOLUTE);
      vgc.line_to(ground_path, points[3], VG_ABSOLUTE);
      vgc.close(ground_path);
    }

  };

  class Curve : public TrackTile {
    Real _radius;
    Real _degrees;
  public:

    static Vector center_point(ConnectionPoint start, Real radius, Real tile_width) {
      Real sign = SIGN(radius); // left turning == positive sign
      Vector center_point(0, sign);
      center_point *= tile_width / 2.0 + fabs(radius);
      AffineTransform r = rotation(start.direction);
      return (r * center_point) + start.point;
    }

    Curve(const Json::Value& tile, const ConnectionPoint& start, const TileInfo& ti) 
      : TrackTile(ti)
    {
      assert(tile.isMember("radius") && tile["radius"].isDouble());
      assert(tile.isMember("degrees") && tile["degrees"].isDouble());
      _start = start;
      _radius = tile["radius"].asDouble();
      _degrees = tile["degrees"].asDouble();
      Vector center_point = Curve::center_point(start, _radius, _ti.width());
      Vector swipe = start.point - center_point;
      _end.point = (rotation(_degrees) * swipe) + center_point;
      _end.direction = start.direction + _degrees;
    }

    virtual void append_to_ground_path(const OpenVGCompanion& vgc, VGPath ground_path) const {
      Real w = _ti.width();
      Vector offset(0, w / 2.0);
      AffineTransform t = rotation(_start.direction);
      Vector start_offset = t * offset;
      t = rotation(_end.direction);
      Vector end_offset = t * offset;
      Vector p1 = _start.point + start_offset;
      Vector p2 = _end.point + end_offset;
      Vector p3 = _end.point - end_offset;
      Vector p4 = _start.point - start_offset;
      vgc.move_to(ground_path, p1, VG_ABSOLUTE);
      vgc.arc(ground_path, VG_SCCWARC_TO, p2, _radius, _radius, _degrees, VG_ABSOLUTE);
      vgc.line_to(ground_path, p3, VG_ABSOLUTE);
      vgc.arc(ground_path, VG_SCWARC_TO, p4, _radius + w, _radius + w, _degrees, VG_ABSOLUTE);
      vgc.close(ground_path);
    }

  };

  shared_ptr<TrackTile> TrackTile::create_tile(const Json::Value& tile, const ConnectionPoint& start, const TileInfo &ti) {
    assert(tile.isMember("type"));
    string type = tile["type"].asString();
    if(type == "startinggrid") {
      return shared_ptr<TrackTile>(new Straight(tile, start, ti));
    } else if(type == "curve") {
      return shared_ptr<TrackTile>(new Curve(tile, start, ti));
    } else if(type == "straight") {
      return shared_ptr<TrackTile>(new Straight(tile, start, ti));
    } else {
      assert(false);
    }
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
    VGPath ground_path = vgc.newPath();

    VGPaint ground_paint = vg.vgCreatePaint();
    vg.vgSetParameteri(ground_paint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
    VGfloat color_components[4] = {.5, .5, .5, 1.0};
    vg.vgSetParameterfv(ground_paint, VG_PAINT_COLOR, 4, color_components);
    vg.vgSetPaint(ground_paint, VG_FILL_PATH|VG_STROKE_PATH);

    BOOST_FOREACH(shared_ptr<TrackTile> tile, _tiles) {
      tile->append_to_ground_path(vgc, ground_path);
    }

    vg.vgDrawPath(ground_path, VG_FILL_PATH);
    vg.vgDestroyPath(ground_path);
    vg.vgDestroyPaint(ground_paint);
  }

} // namespace rracer
