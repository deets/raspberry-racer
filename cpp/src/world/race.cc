#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>
#include "world/race.hh"
#include "world/track.hh"
#include "world/car.hh"

namespace rracer {

  CarInfo::CarInfo(Car* car, Track* track, int number) 
    : car(car)
    , track(track)
  {
    lane = number % track->number_of_lanes();
    box = number / track->number_of_lanes();
    ConnectionPoint point;
    tie(point, tile) = track->starting_position(lane, box);
    car->place(point);
  }

  void CarInfo::process_input_events(const InputEventVector& events, double elapsed) {
    const Vector car_pos = car->position().point;
    NearestPointInfo npi = tile->nearest_point(lane, car_pos);
    if(npi.offset > 1.0) {
      tile = tile->next();
      npi = tile->nearest_point(lane, car_pos);
    } else if(npi.offset < 0.0) {
      tile = tile->prev();
      npi = tile->nearest_point(lane, car_pos);
    }
    assert(npi.offset >= 0 && npi.offset <= 1.0);
    car->push_to_slot(npi.point);
  }

  Race::Race(World& world, AssetManager& asset_manager, const string& track_name, const string& car_name) 
  : _world(world)
  , _asset_manager(asset_manager)
  {
    rracer::Track* track = new rracer::Track(_asset_manager, track_name);
    rracer::AffineTransformator* t = new rracer::AffineTransformator(_world.screen_rect().fit(track->bounds() * 1.1));
    rracer::Car* car = new rracer::Car(_asset_manager, _asset_manager.json(car_name));
    car->physics_setup(_world.world());
    t->add_object(track);
    t->add_object(car);
    _world.add_object(t);

    CarInfo car_info(car, track, 0);
    _cars.push_back(car_info);

    // _hud = new rracer::HUD(
    //     rracer::Vector(20, size.height - 20),
    //     _asset_manager.font(),
    //     *_window_adapter,
    //     _world,
    //     boost::bind(&rracer::AffineTransformator::affine_transform, t)
    // );
    // _world->add_object(_hud);
    
    // rracer::ConnectionPoint point = { rracer::Vector(10, 10), 45 };
    // rracer::KeyAction* resetter = new rracer::KeyAction(
    // 	rracer::K_SPACE,
    // 	boost::bind(&rracer::Car::place, car, point)
    // );

    // _world->add_object(resetter);

  }

  void Race::process_input_events(const InputEventVector& events, double elapsed) {
    BOOST_FOREACH(CarInfo& ci, _cars) {
      ci.process_input_events(events, elapsed);
    }
  }

} // end ns::rracer
