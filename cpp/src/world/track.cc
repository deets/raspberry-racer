#include "json/json.h"
#include "world/track.hh"


class StartingGrid : public TrackTile {
  ConnectionPoint _start;
  ConnectionPoint _end;
public:
  StartingGrid(const Json::Value& tile, const ConnectionPoint& start) 
    : _start(start)
  {
    assert(tile.isMember("length") && tile["length"].isDouble());
    Real length = tile["length"].asDouble();
    AffineTransform t = AffineTransform::Identity();
    t = t.rotate(_start.direction);
    Vector l;
    l << length, 0;
    l = t * l;
    _end.point = _start.point + l;
    _end.direction = _start.direction;
  }

  virtual ConnectionPoint start() const {
    return _start;
  }

  virtual ConnectionPoint end() const {
    return _end;
  }
};


shared_ptr<TrackTile> TrackTile::create_tile(const Json::Value& tile, const ConnectionPoint& start) {
  assert(tile.isMember("type") && tile["type"].asString() == "startinggrid");
  string type = tile["type"].asString();
  if(type == "startinggrid") {
    return shared_ptr<TrackTile>(new StartingGrid(tile, start));
  }
}

Track::Track(AssetManager& am, fs::path data) 
  : _am(am)
{
  Json::Value json = am.json(data);
  assert(json.isMember("type") && json["type"].asString() == "simple");
  assert(json.isMember("name") && json["name"].isString());
  assert(json.isMember("lanes") && json["lanes"].isInt() && json["lanes"] > 0);
  assert(json.isMember("tiles") && json["tiles"].isArray());

  _name = json["name"].asString();

  ConnectionPoint connection_point;
  connection_point.point << 0.0, 0.0;
  connection_point.direction = 0.0;

  for(int i=0; i < json["tiles"].size(); ++i) {
    Json::Value tile_json = json["tiles"][i];
    assert(tile_json.isObject());
    shared_ptr<TrackTile> tile = TrackTile::create_tile(tile_json, connection_point);
    _tiles.push_back(tile);
  }
  
}

Track::~Track() {

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

