#ifndef TRACK_TILES_HH
#define TRACK_TILES_HH

#include <vector>
#include <boost/shared_ptr.hpp>

#include "gfx/openvg-companion.hh"
#include "common/common.hh"
#include "json/json-forwards.h"

using namespace boost;
using namespace std;


namespace rracer {
  class TileInfo {
    vector<Real> _lane_offsets;
    Real _width;
    Real _slot_width;

  public:
    TileInfo(const Json::Value tile_info);

    Real width() const {
      return _width;
    }

    Real slot_width() const {
      return _slot_width;
    }

    /**
     * Returns the line-offset.
     * Positive values are left,
     * Negative right.
     */
    Real operator[](const size_t index) const {
      assert(index < _lane_offsets.size());
      return _lane_offsets[index];
    }

    int number_of_lanes() const {
      return _lane_offsets.size();
    }

  };

  struct NearestPointInfo {
    int lane;
    Vector point;
    Real offset; // the offset in tile-parametrization
    Real distance;

    bool operator<(const NearestPointInfo& other) const {
      return fabs(this->distance) < fabs(other.distance);
    }

  };

  class TrackTile {


  public:
    TrackTile(const TileInfo&);
    static shared_ptr<TrackTile> create_tile(const Json::Value&, const ConnectionPoint&, const TileInfo&);
    virtual ConnectionPoint start() const;
    virtual ConnectionPoint end() const;
    virtual void render(OpenVGCompanion&, const Color& ground_color) const=0;
    virtual const Rect bounds() const;
    virtual Vector position(Real offset, int lane) const=0;
    virtual NearestPointInfo nearest_point(int lane, const Vector&) const=0;
  protected:
    Rect _bounds;
    const TileInfo& _ti;
    ConnectionPoint _start;
    ConnectionPoint _end;
  };


  class Straight : public TrackTile {
  public:
    Straight(const Json::Value& tile, const ConnectionPoint& start, const TileInfo& ti);
    virtual void render(OpenVGCompanion&, const Color& ground_color) const;
    virtual Vector position(Real offset, int lane) const;
    virtual NearestPointInfo nearest_point(int lane, const Vector&) const;
  protected:
    Real _length;
    const vector<Vector> corners() const;
  };

  class Curve : public TrackTile {
    Real _radius;
    Real _degrees;
  public:
    static Vector center_point(ConnectionPoint start, Real radius, Real tile_width);

    Curve(const Json::Value& tile, const ConnectionPoint& start, const TileInfo& ti);
    virtual void render(OpenVGCompanion&, const Color& ground_color) const;
    virtual Vector position(Real offset, int lane) const;
    virtual NearestPointInfo nearest_point(int lane, const Vector&) const;
  };


  class StartingGrid : public Straight {
    struct StartBoxInfo {
      Real length; // in centimeters, determines spacing in multi-car-per-lane
      Real width; // in 0..1 of the whole lane width
      Real finish_line_offset; // offset from the actual finish-line
    };

  public:
    StartingGrid(const Json::Value& tile, const ConnectionPoint& start, const TileInfo& ti);
    virtual void render(OpenVGCompanion&, const Color& ground_color) const;
    ConnectionPoint starting_position(int lane, int box) const;
  private:
    StartBoxInfo _startbox;
  };
}; // end ns::rracer

#endif
