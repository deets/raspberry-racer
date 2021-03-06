#include <boost/foreach.hpp>
#include "json/json.h"
#include "scene/track-tiles.hh"


namespace rracer {

  void TrackTile::connect(TrackTile* next) {
    _next = next;
    next->_prev = this;
  }


  TrackTile* TrackTile::next() const {
    return _next;
  }


  TrackTile* TrackTile::prev() const {
    return _prev;
  }


  string TrackTile::name() const {
    return _name;
  }


  void TrackTile::name(const string n) {
    _name = n;
  }


  Straight::Straight(const Json::Value& tile, const ConnectionPoint& start, const TileInfo& ti)
      : TrackTile(ti)
    {
      assert(tile.isMember("length") && tile["length"].isDouble());
      _length = tile["length"].asDouble();
      AffineTransform t = rotation(start.direction);
      Vector l(_length, 0);
      l = t * l;
      _start = start;
      _end.position = _start.position + l;
      _end.direction = _start.direction;
      _bounds = Rect::from_points(corners());
    }


  const vector<Vector> Straight::corners() const {
    vector<Vector> res;
    Real whalf = _ti.width() / 2.0;
    Vector offset(0, whalf);
    AffineTransform t = rotation(start().direction);
    offset = t * offset;
    const Vector start = this->start().position;
    const Vector end =  this->end().position;
    res.push_back(start + offset);
    res.push_back(end + offset);
    res.push_back(end - offset);
    res.push_back(start - offset);
    return res;
  }

  NearestPointInfo Straight::nearest_point(int lane, const Vector& pivot_position) const {
    NearestPointInfo npi;
    npi.lane = lane;
    const Vector start = position(0, lane);
    const Vector n = rotation(_start.direction) * Vector(0, 1.0);
    npi.distance = fabs((pivot_position - start).dot(n));
    const Vector d = rotation(_start.direction) * Vector(1.0, 0);
    npi.offset = (pivot_position - start).dot(d) / _length;
    npi.point.position = start + (d * npi.offset * _length);
    npi.point.direction = _start.direction;
    return npi;
  }

  void Straight::render(OpenVGCompanion& vgc, const Color& ground_color) const {
    {
      PaintScope c(vgc, ground_color, VG_FILL_PATH);
      PathScope p(vgc, VG_FILL_PATH);
      const vector<Vector> points = corners();
      vgc.move_to(p, points[0], VG_ABSOLUTE);
      vgc.line_to(p, points[1], VG_ABSOLUTE);
      vgc.line_to(p, points[2], VG_ABSOLUTE);
      vgc.line_to(p, points[3], VG_ABSOLUTE);
      vgc.close(p);
    }
    // Render slots
    {
      PaintScope c(vgc, Color::black, VG_STROKE_PATH);
      PathScope p(vgc, VG_STROKE_PATH);
      vgc.stroke_width(_ti.slot_width());
      for(int lane = 0; lane < _ti.number_of_lanes(); ++lane) {
	vgc.move_to(p, position(0, lane), VG_ABSOLUTE);
	vgc.line_to(p, position(1.0, lane), VG_ABSOLUTE);
      }
    }

  }


  Vector Straight::position(Real offset, int lane) const {
    assert(offset >= 0 && offset <= 1.0);
    assert(lane >= 0 && lane < _ti.number_of_lanes());
    const Vector lane_offset = rotation(_start.direction) * Vector(0, _ti[lane]);
    return _start.position + (_end.position - _start.position) * offset + lane_offset;
  }

  //======================================================================


  /**
   * Computes the curve's center point (+):
   *
   * +   |    |
   * ___/    /
   *   	    /
   * ______/
   *
   */

  Vector Curve::center_point(ConnectionPoint start, Real radius, Real tile_width) {
    Real sign = SIGN(radius); // left turning == positive sign
    Vector center_point(0, sign);
    center_point *= tile_width / 2.0 + fabs(radius);
    AffineTransform r = rotation(start.direction);
    return (r * center_point) + start.position;
  }


  NearestPointInfo Curve::nearest_point(int lane, const Vector& pivot_position) const {
    const Vector cp = center_point(_start, _radius, _ti.width());
    const Vector swipe = _start.position - cp;
    const Vector norm = swipe / swipe.norm();
    const Vector p = pivot_position - cp;
    const Real angle = ::acos(norm.dot(p / p.norm())) / M_PI * 180;
    const Real offset = angle / _degrees;
    const AffineTransform r = rotation(angle);
    const Real tile_offset = _ti.width() / 2.0 + _radius - _ti[lane];
    const ConnectionPoint point = { r * norm * tile_offset + cp, _start.direction + angle };
    NearestPointInfo npi = { 
      lane,
      point,
      offset,
      (point.position - pivot_position).norm()
    };
    return npi;
  }


  Curve::Curve(const Json::Value& tile, const ConnectionPoint& start, const TileInfo& ti)
      : TrackTile(ti)
    {
      assert(tile.isMember("radius") && tile["radius"].isDouble());
      assert(tile.isMember("degrees") && tile["degrees"].isDouble());
      _start = start;
      _radius = tile["radius"].asDouble();
      _degrees = tile["degrees"].asDouble();
      Vector center_point = Curve::center_point(start, _radius, _ti.width());
      Vector swipe = start.position - center_point;
      _end.position = (rotation(_degrees) * swipe) + center_point;
      _end.direction = start.direction + _degrees;

      vector<Vector> points;
      AffineTransform t = rotation(_end.direction);
      Vector full_swipe = swipe / swipe.norm() * (_ti.width() + _radius);
      const Vector p1 = center_point + full_swipe;
      const Vector p2 = center_point + t * full_swipe;
      points.push_back(center_point);
      points.push_back(p1);
      points.push_back(p2);
      EQuarterClass p1class = classify_point(center_point, p1);
      EQuarterClass p2class = classify_point(center_point, p2);
      // when we are either in different quadrants
      // or span all four
      if(p1class != p2class || fabs(_degrees) > 90) {
	if(SIGN(_degrees) == -1) { // clockwise, so we must swap
	  swap(p1class, p2class);
	}
	BOOST_FOREACH(const EQuarterClass &axis, spanning_quadrants(p1class, p2class)) {
	  points.push_back(axis_point(center_point, _radius, axis));
	}
      }
      _bounds = Rect::from_points(points);
    }


  void Curve::render(OpenVGCompanion& vgc, const Color& ground_color) const {
    Real w = _ti.width();
    {
      PaintScope c(vgc, ground_color, VG_FILL_PATH);
      PathScope p(vgc, VG_FILL_PATH);
      Vector offset(0, w / 2.0);
      AffineTransform t = rotation(_start.direction);
      Vector start_offset = t * offset;
      t = rotation(_end.direction);
      Vector end_offset = t * offset;
      Vector p1 = _start.position + start_offset;
      Vector p2 = _end.position + end_offset;
      Vector p3 = _end.position - end_offset;
      Vector p4 = _start.position - start_offset;
      vgc.move_to(p, p1, VG_ABSOLUTE);
      vgc.arc(p, VG_SCCWARC_TO, p2, _radius, _radius, _degrees, VG_ABSOLUTE);
      vgc.line_to(p, p3, VG_ABSOLUTE);
      vgc.arc(p, VG_SCWARC_TO, p4, _radius + w, _radius + w, _degrees, VG_ABSOLUTE);
      vgc.close(p);
    }
    // Render slots
    {
      PaintScope c(vgc, Color::black, VG_STROKE_PATH);
      PathScope p(vgc, VG_STROKE_PATH);
      vgc.stroke_width(_ti.slot_width());
      for(int lane = 0; lane < _ti.number_of_lanes(); ++lane) {
	VGfloat r = _radius + w / 2.0 - _ti[lane];
	vgc.move_to(p, position(0, lane), VG_ABSOLUTE);
	vgc.arc(p, VG_SCCWARC_TO, position(1.0, lane), r, r, _degrees, VG_ABSOLUTE);
	vgc.close(p);
      }
    }
  }


  Vector Curve::position(Real offset, int lane) const {
    assert(offset >= 0 && offset <= 1.0);
    assert(lane >= 0 && lane < _ti.number_of_lanes());
    const Vector cp = Curve::center_point(_start, _radius, _ti.width());
    Vector v = (_start.position - cp) / (_ti.width() / 2.0 + _radius);
    v *= _ti.width() / 2.0 + _radius - _ti[lane];
    return cp + (rotation(offset * _degrees) * v);
  }


  StartingGrid::StartingGrid(const Json::Value& tile, const ConnectionPoint& start, const TileInfo& ti) 
    : Straight(tile, start, ti)
  {
    assert(tile.isMember("start-box") && tile["start-box"].isObject());
    Json::Value startbox_json = tile["start-box"];
    assert(startbox_json.isMember("length") && startbox_json["length"].isDouble());
    assert(startbox_json.isMember("width") && startbox_json["width"].isDouble());
    assert(startbox_json.isMember("finish-line-offset") && startbox_json["finish-line-offset"].isDouble());
    _startbox.length = startbox_json["length"].asDouble();
    _startbox.width = startbox_json["width"].asDouble();
    _startbox.finish_line_offset = startbox_json["finish-line-offset"].asDouble();
    assert(_startbox.length > 0 && _startbox.length < this->_length);
    assert(_startbox.width >= 0 && _startbox.width <= 1.0);
    assert(_startbox.finish_line_offset > 0 && _startbox.finish_line_offset < _length);
  }


  void StartingGrid::render(OpenVGCompanion& vgc, const Color& ground_color) const {
    Straight::render(vgc, ground_color);

    // render the checkered start line
    const Real base = _ti.width() / 8.0;
    const AffineTransform r = rotation(_start.direction);
    const Vector step_x = r * Vector(base, 0);
    const Vector step_y = r * Vector(0, base);
    const Vector end = _end.position;
    {
      PaintScope white(vgc, Color(1.0, 1.0, 1.0), VG_FILL_PATH);
      PathScope p(vgc, VG_FILL_PATH);
      for(int i = 0; i < 4; ++i) {
	Vector offset = step_y * (2 - i) * 2 - step_y;
	vgc.move_to(p, end - step_x + offset, VG_ABSOLUTE);
	vgc.line_to(p, end - step_x + step_y + offset, VG_ABSOLUTE);
	vgc.line_to(p, end + step_y + offset, VG_ABSOLUTE);
	vgc.line_to(p, end + offset, VG_ABSOLUTE);
	vgc.close(p);
	offset = offset - step_x - step_y;
	vgc.move_to(p, end - step_x + offset, VG_ABSOLUTE);
	vgc.line_to(p, end - step_x + step_y + offset, VG_ABSOLUTE);
	vgc.line_to(p, end + step_y + offset, VG_ABSOLUTE);
	vgc.line_to(p, end + offset, VG_ABSOLUTE);
	vgc.close(p);
      }
    }
    // render the starting box lines
    const Vector box_half_width = r * Vector(0, 1) * (_ti.width() / 4.0) * _startbox.width;
    const Vector box_length = r * Vector(-_startbox.length, 0);
    const Vector finish_line_offset = r * Vector(-_startbox.finish_line_offset, 0);
    vgc.vg().vgSetf(VG_STROKE_LINE_WIDTH, _ti.width() / 40.0);
    for(int lane = 0; lane < _ti.number_of_lanes(); ++lane) {
      Vector finish_line = position(1.0, lane);
      for(int box_no = 0; box_no < 1; ++box_no) {
	PaintScope white(vgc, Color(1.0, 1.0, 1.0), VG_STROKE_PATH);
	PathScope p(vgc, VG_STROKE_PATH);
	const Vector box_pos = (box_length * box_no) + finish_line_offset + finish_line;
	vgc.move_to(p, box_pos + box_half_width, VG_ABSOLUTE);
	vgc.line_to(p, box_pos - box_half_width, VG_ABSOLUTE);
	vgc.close(p);
      }
    }
  }

  ConnectionPoint StartingGrid::starting_position(int lane, int box) const {
    const AffineTransform r = rotation(_start.direction);
    const Vector box_length = r * Vector(-_startbox.length, 0);
    const Vector finish_line_offset = r * Vector(-_startbox.finish_line_offset, 0);
    Vector finish_line = position(1.0, lane);
    ConnectionPoint res = { finish_line + finish_line_offset + (box_length * box), _start.direction };
    return res;
  }

  // Factory function, declared in track.hh

  shared_ptr<TrackTile> TrackTile::create_tile(const Json::Value& tile, const ConnectionPoint& start, const TileInfo &ti) {
    assert(tile.isMember("type"));
    string type = tile["type"].asString();
    if(type == "startinggrid") {
      return shared_ptr<TrackTile>(new StartingGrid(tile, start, ti));
    } else if(type == "curve") {
      return shared_ptr<TrackTile>(new Curve(tile, start, ti));
    } else if(type == "straight") {
      return shared_ptr<TrackTile>(new Straight(tile, start, ti));
    } else {
      assert(false);
    }
  }



}; // end ns::rracer
