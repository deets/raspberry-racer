#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/range.hpp>
#include <boost/range/algorithm_ext.hpp>

#include "game/game.hh"


namespace rracer {

  GameObject::GameObject() 
    : _parent(NULL)
  {
  }


  GameObject::~GameObject() {
  }


  GameObject::iterator GameObject::begin() {
    return _children.begin();
  }

  
  GameObject::iterator GameObject::end() {
    return _children.end();
  }


  void GameObject::setup_within_world(b2World *) {
  }


  void GameObject::on_object_added(GameObject* parent, GameObject* child) {
  }


  void GameObject::dispatch_frame_events(const GameEventVector& events, const TimeInfo& time_info, EventEmitter emit_event) {
    this->process_frame_events(events, time_info, emit_event);
    BOOST_FOREACH(GameObject& child, _children) {
      child.dispatch_frame_events(events, time_info, emit_event);
    }
  }


  void GameObject::process_frame_events(const GameEventVector& events, const TimeInfo& time_info, EventEmitter emit_event) {
 } 


  void GameObject::object_added(GameObject* parent, GameObject* child) {
    on_object_added(parent, child);
    if(this->parent()) {
      this->parent()->object_added(parent, child);
    }
  }


  GameObject* GameObject::parent() const {
    return _parent;
  }


  void GameObject::parent(GameObject* p) {
    _parent = p;
  }


  void GameObject::add_object(GameObject* child) {
    child->parent(this);
    _children.push_back(*child);
    object_added(this, child);
  }


  //=====================================================

  GameRoot::GameRoot(function<void (GameObject*, GameObject*)> on_object_added_callback) 
    : GameObject()
    , _on_object_added_callback(on_object_added_callback)
  {
  }


  void GameRoot::on_object_added(GameObject* parent, GameObject* child) {
    _on_object_added_callback(parent, child);
  }


  //=====================================================

  Game::Game(SceneGraph* scene_graph) 
    : _scene_graph(scene_graph)
    , _has_ended(false)
  {
    b2Vec2 gravity(.0, 0);
    _world = new b2World(gravity);
    _root = shared_ptr<GameRoot>(new GameRoot(boost::bind(&Game::on_object_added, this, _1, _2)));
  }


  Game::~Game() {
    delete _world;
  }
  
  
  void Game::on_object_added(GameObject* parent, GameObject* child) {
    child->setup_within_world(_world);
  }


  void Game::step(const GameEventVector& events, Real elapsed) {
  
    BOOST_FOREACH(const GameEvent event, events) {
      if(boost::get<KeyEvent>(event.event).key == K_ESC) {
	_has_ended = true;
      }
    }

    const Real step_size = elapsed;

    _time_info = TimeInfo(_time_info.when() + step_size, step_size);

    // prepare the event-vectors.
    GameEventVector this_frame_events(events);
    boost::range::push_back(this_frame_events, _next_frame_events);
    _next_frame_events.clear();

    EventEmitter emit_event = boost::bind(&GameEventVector::push_back, &_next_frame_events, _1);

    for(GameObject::iterator it = _root->begin(); it != _root->end(); ++it) {
      (*it).dispatch_frame_events(this_frame_events, _time_info, emit_event);
    }

    // simulate physics
    _world->Step(_time_info.elapsed(), WORLD_VELOCITY_ITERATIONS, WORLD_POSITION_ITERATIONS);
  }


  void Game::add_object(GameObject* child) {
    _root->add_object(child);
  }


  Game::iterator Game::begin() {
    return iterator(_root->begin(), _root->end());
  }


  Game::iterator Game::end() {
    return iterator(_root->end(), _root->end());
  }

  //===== iterator ========

  Game::iterator::iterator(go_iterator begin, go_iterator end) 
  {
    if(begin == end) {
      return;
    }
    _iterators.push(make_pair(begin, end));
  }


  Game::iterator::iterator(const iterator& other) {
    this->_iterators = other._iterators;
  }


  Game::iterator::~iterator() {
  }


  Game::iterator& Game::iterator::operator=(const iterator& rhs) {
    this->_iterators = rhs._iterators;
    return *this;
  }


  bool Game::iterator::operator==(const iterator& rhs) const {
    return _iterators.size() == 0 && rhs._iterators.size() == 0;
  }


  bool Game::iterator::operator!=(const iterator& rhs) const {
    return !(*this==rhs);
  }


  Game::iterator& Game::iterator::operator++() {
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
  
  Game::iterator::reference Game::iterator::operator*() const {
    return *_iterators.top().first;
  }


  Game::iterator::pointer Game::iterator::operator->() const {
  }

} // end ns::rracer
