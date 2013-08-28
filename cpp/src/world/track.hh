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


typedef struct {
  Vector point;
  Real direction;
} ConnectionPoint;

class TrackTile {

public:
  static shared_ptr<TrackTile> create_tile(const Json::Value&, const ConnectionPoint&);
  virtual ConnectionPoint start() const=0;
  virtual ConnectionPoint end() const=0;
};


class Track : public WorldObject {

  AssetManager& _am;
public:
  Track(AssetManager& am, fs::path data);
  virtual ~Track();

  const string& name() const;

  size_t count() const;

  TrackTile& operator[](size_t index) const;

private:
  string _name;
  vector<shared_ptr<TrackTile> > _tiles;
};


#endif
