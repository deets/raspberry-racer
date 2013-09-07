#ifndef TRACK_HH
#define TRACK_HH
#include <string>

#include <vector>
#include <boost/shared_ptr.hpp>

#include "json/json-forwards.h"

#include "common/common.hh"
#include "world/world-object.hh"
#include "assets/assets.hh"


using namespace std;
using namespace boost;;

namespace rracer {
  typedef struct {
    Vector point;
    Real direction;
  } ConnectionPoint;


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

  class TrackTile {


  public:
    TrackTile(const TileInfo&);
    static shared_ptr<TrackTile> create_tile(const Json::Value&, const ConnectionPoint&, const TileInfo&);
    virtual ConnectionPoint start() const;
    virtual ConnectionPoint end() const;
    virtual void render(OpenVGCompanion&, const Color& ground_color) const=0;
    virtual const Rect bounds() const;
    virtual Vector position(Real offset, int lane) const=0;

  protected:
    Rect _bounds;
    const TileInfo& _ti;
    ConnectionPoint _start;
    ConnectionPoint _end;
  };


  class Track : public WorldObject {

    AssetManager& _am;
    shared_ptr<TileInfo> _tile_info;

  public:
    Track(AssetManager& am, fs::path data);
    virtual ~Track();

    const string& name() const;
    size_t count() const;
    TrackTile& operator[](size_t index) const;

    virtual void render(OpenVGCompanion& vgc) const;

    const Rect bounds() const;

  private:
    string _name;
    Rect _bounds;
    vector<shared_ptr<TrackTile> > _tiles;
  };

}
#endif
