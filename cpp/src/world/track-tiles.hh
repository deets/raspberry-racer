#ifndef TRACK_TILES_HH
#define TRACK_TILES_HH
#include "world/track.hh"

namespace rracer {
  class Straight : public TrackTile {
    const vector<Vector> corners() const;
  public:
    Straight(const Json::Value& tile, const ConnectionPoint& start, const TileInfo& ti);
    virtual void render(const OpenVGCompanion&, const Color& ground_color) const;
    virtual Vector position(Real offset, int lane) const;
  };

  class Curve : public TrackTile {
    Real _radius;
    Real _degrees;
  public:
    static Vector center_point(ConnectionPoint start, Real radius, Real tile_width);

    Curve(const Json::Value& tile, const ConnectionPoint& start, const TileInfo& ti);
    virtual void render(const OpenVGCompanion&, const Color& ground_color) const;
    virtual Vector position(Real offset, int lane) const;
  };


  class StartingGrid : public Straight {
  public:
    StartingGrid(const Json::Value& tile, const ConnectionPoint& start, const TileInfo& ti);
    virtual void render(const OpenVGCompanion&, const Color& ground_color) const;
  };
}; // end ns::rracer

#endif
