#ifndef TRACK_TILES_HH
#define TRACK_TILES_HH
#include "world/track.hh"

namespace rracer {
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
  private:
    StartBoxInfo _startbox;
  };
}; // end ns::rracer

#endif
