#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/bind.hpp>
#include <boost/log/trivial.hpp>
#include <boost/range.hpp>
#include <boost/range/algorithm_ext.hpp>
#include "world/race.hh"
#include "world/track.hh"
#include "world/car.hh"
#include "debug/hud.hh"

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


  Vector CarInfo::car_position() const {
    return car->position().position;
  }


  ConnectionPoint CarInfo::slot_position(InputEventVector& next_frame_events) {
    const Vector car_pos = car_position();
    NearestPointInfo npi = tile->nearest_point(lane, car_pos);
    if(npi.offset > 1.0) {
      tile = tile->next();
      npi = tile->nearest_point(lane, car_pos);
    } else if(npi.offset < 0.0) {
      tile = tile->prev();
      npi = tile->nearest_point(lane, car_pos);
    }
    assert(npi.offset >= 0 && npi.offset <= 1.0);    
    BOOST_LOG_TRIVIAL(trace) << tile->name() << " dir: " << npi.point.direction; 
    return npi.point;
  }


  InputEventVector CarInfo::process_input_events(const InputEventVector& events, double elapsed) {
    InputEventVector next_frame_events;
    car->push_to_slot(slot_position(next_frame_events));
    return next_frame_events;
  }


  Race::Race(World& world, AssetManager& asset_manager, const string& track_name, const string& car_name) 
  : _world(world)
  , _asset_manager(asset_manager)
  {
    struct local {
      static Vector to_vector(function<ConnectionPoint () > cpf) {
	return cpf().position;
      }
    };

    rracer::Track* track = new rracer::Track(_asset_manager, track_name);
    rracer::AffineTransformator* t = new rracer::AffineTransformator(_world.screen_rect().fit(track->bounds() * 1.1));
    rracer::Car* car = new rracer::Car(_asset_manager, _asset_manager.json(car_name));
    car->physics_setup(_world.world());
    t->add_object(track);
    t->add_object(car);
    _world.add_object(t);

    CarInfo car_info(car, track, 0);
    _cars.push_back(car_info);

    HUD* hud = new HUD(
        rracer::Vector(20, world.screen_rect().height() - 20),
        _asset_manager.font(),
        _world,
        boost::bind(&rracer::AffineTransformator::affine_transform, t)
    );

    _world.add_object(hud);
  }

  InputEventVector Race::process_input_events(const InputEventVector& events, double elapsed) {
    InputEventVector next_frame_events;
    BOOST_FOREACH(CarInfo& ci, _cars) {
      push_back(next_frame_events, ci.process_input_events(events, elapsed));
    }
    return next_frame_events;
  }

} // end ns::rracer









