#ifndef RACE_HH
#define RACE_HH
#include "assets/assets.hh"
#include "world/world.hh"
#include "world/track.hh"
#include "world/car.hh"

namespace rracer {

  class CarInfo {
  public:
    CarInfo(Car* car, Track*, int number);

    void process_frame_events(const GameEventVector& events, const TimeInfo& time_info, EventEmitter emit_event);

    Vector car_position() const;
    ConnectionPoint slot_position(EventEmitter emit_event);

    Car* car;
    Track* track;
    TrackTile* tile;
    int lane;
    int number;
    int box;
  };

  class Race : public WorldObject {
  public:
    Race(World& world, AssetManager&, const string& track_name, const string& car_name);
    virtual void process_frame_events(const GameEventVector& events, const TimeInfo& time_info, EventEmitter emit_event);

  private:
    World& _world;
    AssetManager& _asset_manager;

    vector<CarInfo> _cars;
  };


}; // end ns::rracer


#endif
