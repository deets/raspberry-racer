#ifndef TRACK_HH
#define TRACK_HH
#include <string>

#include <vector>
#include <boost/shared_ptr.hpp>

#include "json/json-forwards.h"

#include "common/common.hh"
#include "world/world-object.hh"
#include "assets/assets.hh"
#include "world/track-tiles.hh"

using namespace std;
using namespace boost;

namespace rracer {

  class Track : public WorldObject {
  public:
    Track(AssetManager& am, fs::path data);
    virtual ~Track();

    const string& name() const;
    size_t count() const;
    TrackTile& operator[](size_t index) const;

    virtual void render(OpenVGCompanion& vgc) const;

    const Rect bounds() const;
    int number_of_lanes() const;

    pair<ConnectionPoint, TrackTile*> starting_position(int lane, int box) const;

    pair<NearestPointInfo, TrackTile*> locate_slot_anchor(int lane, const Vector& slot) const;

  private:
    AssetManager& _am;
    string _name;
    Rect _bounds;
    shared_ptr<TileInfo> _tile_info;
    vector<shared_ptr<TrackTile> > _tiles;
    StartingGrid* _starting_grid;
  };

}
#endif
