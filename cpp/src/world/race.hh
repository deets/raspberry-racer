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

    /**
     * Manages the car.
     * Will before physics step
     * ensure the car is being
     * pushed towards the slot,
     * and possibly deal with derailing.
     */
    void process_frame_events(const GameEventVector& events, const TimeInfo& time_info, EventEmitter emit_event);

    /**
     * Position of the managed car
     */
    Vector car_position() const;

    /**
     * Based on the current car-position,
     * return the nearest position on a
     * slot.
     *
     * Potentially this call will emit
     *
     * CarInfoEvents
     *
     * to inform other parts of the application
     * that the car has moved from tile to tile.
     */
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
    virtual void physics_setup(b2World *physics_world);
    virtual void process_frame_events(const GameEventVector& events, const TimeInfo& time_info, EventEmitter emit_event);

  private:
    World& _world;
    AssetManager& _asset_manager;
    string _track_name;
    string _car_name;
    vector<CarInfo> _cars;
  };


}; // end ns::rracer


#endif
