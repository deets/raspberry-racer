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
public:
  TileInfo(const Json::Value tile_info);

  Real width() const {
    return _width;
  }
  Real operator[](const size_t index) const {
    assert(index < _lane_offsets.size());
    return _lane_offsets[index];
  }
};

class TrackTile {

public:
  static shared_ptr<TrackTile> create_tile(const Json::Value&, const ConnectionPoint&, const TileInfo&);
  virtual ConnectionPoint start() const=0;
  virtual ConnectionPoint end() const=0;
  virtual void append_to_ground_path(const OpenVGCompanion&, VGPath ground_path) const=0;
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

  virtual void render(const OpenVGCompanion& vgc) const;

private:
  string _name;
  vector<shared_ptr<TrackTile> > _tiles;
};

}
#endif
