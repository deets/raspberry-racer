#include <boost/foreach.hpp>
#include "json/json.h"
#include "world/track-tiles.hh"

namespace rracer {
  Straight::Straight(const Json::Value& tile, const ConnectionPoint& start, const TileInfo& ti)
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

      _bounds = Rect::from_points(corners());

    }

  const vector<Vector> Straight::corners() const {
    vector<Vector> res;
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

  void Straight::append_to_ground_path(const OpenVGCompanion& vgc, VGPath ground_path) const {
    const vector<Vector> points = corners();
    vgc.move_to(ground_path, points[0], VG_ABSOLUTE);
    vgc.line_to(ground_path, points[1], VG_ABSOLUTE);
    vgc.line_to(ground_path, points[2], VG_ABSOLUTE);
    vgc.line_to(ground_path, points[3], VG_ABSOLUTE);
    vgc.close(ground_path);
  }


  Vector Straight::position(Real offset, int lane) const {
    assert(offset >= 0 && offset <= 1.0);
    assert(lane >= 0 && lane < _ti.number_of_lanes());
    const Vector lane_offset = rotation(_start.direction) * Vector(0, _ti[lane]);
    return _start.point + (_end.point - _start.point) * offset + lane_offset;
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
    return (r * center_point) + start.point;
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
      Vector swipe = start.point - center_point;
      _end.point = (rotation(_degrees) * swipe) + center_point;
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

  void Curve::append_to_ground_path(const OpenVGCompanion& vgc, VGPath ground_path) const {
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


  Vector Curve::position(Real offset, int lane) const {
    assert(offset >= 0 && offset <= 1.0);
    assert(lane >= 0 && lane < _ti.number_of_lanes());
    const Vector cp = Curve::center_point(_start, _radius, _ti.width());
    Vector v = (_start.point - cp) / (_ti.width() / 2.0 + _radius);
    v *= _ti.width() / 2.0 + _radius - _ti[lane];
    return cp + (rotation(offset * _degrees) * v);
  }


  StartingGrid::StartingGrid(const Json::Value& tile, const ConnectionPoint& start, const TileInfo& ti) 
    : Straight(tile, start, ti)
  {
  }

  void StartingGrid::append_to_ground_path(const OpenVGCompanion& vgc, VGPath ground_path) const {
    const OpenVGAdapter& vg = vgc.vg();
    Straight::append_to_ground_path(vgc, ground_path);
    return;
    // VGPath checkers_path = vgc.newPath();
    // Real base = _ti.width() / 8.0;
    // VGfloat black[] = {.0, .0, .0, 1.0};
    // VGfloat white[] = {1.0, 1.0, 1.0, 1.0};
    // Vector step_x = rotation(_start.direction) * Vector(base, 0);
    // Vector step_y = rotation(_start.direction) * Vector(0, base);
    // Vector end = _end.point;
    // // VGPaint white_paint = vg.vgCreatePaint();
    // // vg.vgSetParameterfv(white_paint, VG_PAINT_COLOR, 4, white);
    // // vg.vgSetPaint(white_paint, VG_FILL_PATH);
    // vgc.setFillColor(white);
    // vgc.move_to(checkers_path, end - step_x, VG_ABSOLUTE);
    // vgc.line_to(checkers_path, end - step_x + step_y, VG_ABSOLUTE);
    // vgc.line_to(checkers_path, end + step_y, VG_ABSOLUTE);
    // vgc.line_to(checkers_path, end, VG_ABSOLUTE);
    // vg.vgDrawPath(checkers_path, VG_FILL_PATH);
    // vg.vgDestroyPath(checkers_path);
    // vg.vgDestroyPaint(white_paint);
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
