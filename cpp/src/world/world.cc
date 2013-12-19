#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/range.hpp>
#include <boost/range/algorithm_ext.hpp>
#include "gfx/openvg-companion.hh"
#include "world.hh"


namespace rracer {


  //================================================

  World::World(WindowAdapter &window_adapter, OpenVGAdapter &ovg_adapter)
    : _window_adapter(window_adapter)
    , _ovg_adapter(ovg_adapter)
    , _has_ended(false)
    , _debug_renderer(0)
    , _fixed_frame_rate(0.0)
    , _time_info()
  {
    b2Vec2 gravity(.0, 0);
    _world = new b2World(gravity);
  }


  World::~World() {
    delete _world;
  }


  void World::set_debug_renderer(DebugRenderer* dr) {
    _debug_renderer = dr;
    _world->SetDebugDraw(dr);
  }


  Real World::fixed_frame_rate() const {
    return _fixed_frame_rate;
  }


  void World::fixed_frame_rate(Real fixed_frame_rate) {
    _fixed_frame_rate = fixed_frame_rate;
  }


  b2World* World::world() const {
    return _world;
  }


  Rect World::screen_rect() const {
    pair<int, int> window_dims = _window_adapter.window_dimensions();
    return Rect(0, 0, window_dims.first, window_dims.second);
  }


  void World::start_frame(const GameEventVector& events, const Real elapsed) {
    _window_adapter.start();

    BOOST_FOREACH(const GameEvent event, events) {
      if(boost::get<KeyEvent>(event.event).key == K_ESC) {
	_has_ended = true;
      }
    }
    const Real step_size = _fixed_frame_rate == 0.0 ? elapsed : 1.0 / _fixed_frame_rate;

    _time_info = TimeInfo(_time_info.when() + step_size, step_size);

    // prepare the event-vectors.
    GameEventVector this_frame_events(events);
    boost::range::push_back(this_frame_events, _next_frame_events);
    _next_frame_events.clear();

    EventEmitter emit_event = boost::bind(&GameEventVector::push_back, &_next_frame_events, _1);
    BOOST_FOREACH(WorldObject& obj, _world_objects) {
      obj.dispatch_input_events(this_frame_events, _time_info, emit_event);
    }
    // simulate physics
    _world->Step(_time_info.elapsed(), WORLD_VELOCITY_ITERATIONS, WORLD_POSITION_ITERATIONS);
    // render the game objects
    render();
  }

  void World::render() {
    OpenVGCompanion vgc(_ovg_adapter);
  
    VGint ww = _window_adapter.window_dimensions().first;
    VGint wh = _window_adapter.window_dimensions().second;
    VGfloat black[] = {0.0,0,0,1.0};
    VGfloat red[] = {1.0,0,0,1.0};
    _ovg_adapter.vgSetfv(VG_CLEAR_COLOR, 4, black);
    _ovg_adapter.vgClear(0, 0, ww, wh);


    _ovg_adapter.vgLoadIdentity();

    BOOST_FOREACH(WorldObject& obj, _world_objects) {
      obj.dispatch_render(vgc);
    }

    if(_debug_renderer) {
      _debug_renderer->render(_world);
      for(World::iterator it = begin(); it != end(); ++it) {
	(*it).debug_render(*_debug_renderer);
      }
    }
  }

  OpenVGAdapter& World::vg() const {
    return _ovg_adapter;
  }


  void World::end_frame() {
    _window_adapter.end();
  }


  void World::add_object(WorldObject* obj) {
    _world_objects.push_back(*obj);
    obj->physics_setup(world());
  }


  World::iterator World::begin() {
    return iterator(_world_objects.begin(), _world_objects.end());
  }


  World::iterator World::end() {
    return iterator(_world_objects.end(), _world_objects.end());
  }

  // World::const_iterator World::begin() const {
  //   return const_iterator(true);
  // }


  // World::const_iterator World::end() const {
  //   return const_iterator(true);
  // }


  //===== iterator ========

  World::iterator::iterator(wo_iterator begin, wo_iterator end) 
  {
    if(begin == end) {
      return;
    }
    _iterators.push(make_pair(begin, end));
  }


  World::iterator::iterator(const iterator& other) {
    this->_iterators = other._iterators;
  }


  World::iterator::~iterator() {
  }


  World::iterator& World::iterator::operator=(const iterator& rhs) {
    this->_iterators = rhs._iterators;
    return *this;
  }


  bool World::iterator::operator==(const iterator& rhs) const {
    return _iterators.size() == 0 && rhs._iterators.size() == 0;
  }


  bool World::iterator::operator!=(const iterator& rhs) const {
    return !(*this==rhs);
  }


  World::iterator& World::iterator::operator++() {
    if(_iterators.size()) {
      reference current = **this;
      // if we *have* children,
      // the first of them is the next current
      if(current.begin() != current.end()) {
	_iterators.push(make_pair(current.begin(), current.end()));
      } else {
	// we don't have children, so advance the top
	// begin iterator until
	//  - it's not the end, then it's the next current
	//  - it's the end, then pop and re-do
	//  - we're out of iterators, then we're finished
	while(true) {
	  ++(_iterators.top().first);
	  if(_iterators.top().first != _iterators.top().second) {
	    return *this; // we're pointing to the next child
	  }
	  _iterators.pop();
	  if(_iterators.size() == 0) {
	    // we are finished, because
	    // there is no iterator to come.
	    return *this;
	  }
	}
      }
    }
    return *this;
  }
  
  World::iterator::reference World::iterator::operator*() const {
    return *_iterators.top().first;
  }


  World::iterator::pointer World::iterator::operator->() const {
  }

  // ===== iterator const =====

  // World::const_iterator::const_iterator(bool end) 
  //   : _end(end)
  // {
  // }


  // World::const_iterator::const_iterator(const const_iterator& other) {
  //   this->_end = other._end;
  // }


  // World::const_iterator::~const_iterator() {
  // }


  // World::const_iterator& World::const_iterator::operator=(const const_iterator& rhs) {
  //   this->_end = rhs._end;
  //   return *this;
  // }


  // bool World::const_iterator::operator==(const const_iterator& rhs) const {
  //   return _end == rhs._end;
  // }


  // bool World::const_iterator::operator!=(const const_iterator& rhs) const {
  //   return !(*this==rhs);
  // }


  // World::const_iterator& World::const_iterator::operator++() {
  //   return *this;
  // }
  

  // World::const_iterator::reference World::const_iterator::operator*() const {
  //   return *const_cast<value_type*>(_current);
  // }


  // World::const_iterator::pointer World::const_iterator::operator->() const {
  // }

}; // ns::rracer
