#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/range.hpp>
#include <boost/range/algorithm_ext.hpp>
#include "gfx/openvg-companion.hh"
#include "scene-graph.hh"


namespace rracer {

  //================================================

  SceneRoot::SceneRoot() // function<void (SceneNode*, SceneNode*)> on_object_added_callback) 
    : SceneNode()
      //, _on_object_added_callback(on_object_added_callback)
  {
  }


  // void SceneRoot::on_object_added(SceneNode* parent, SceneNode* child) {
  //   _on_object_added_callback(parent, child);
  // }


  //================================================

  SceneGraph::SceneGraph(WindowAdapter &window_adapter, OpenVGAdapter &ovg_adapter)
    : _window_adapter(window_adapter)
    , _ovg_adapter(ovg_adapter)
    , _debug_renderer(0)
    , _fixed_frame_rate(0.0)
  {
    _root = shared_ptr<SceneRoot>(new SceneRoot());//boost::bind(&SceneGraph::on_object_added, this, _1, _2)));
  }

  SceneGraph::~SceneGraph() {
  }


  // void SceneGraph::on_object_added(SceneNode* parent, SceneNode* child) {
  //   child->setup_within_world(_world);
  // }


  void SceneGraph::set_debug_renderer(DebugRenderer* dr) {
    _debug_renderer = dr;
    //_world->SetDebugDraw(dr);
  }


  Real SceneGraph::fixed_frame_rate() const {
    return _fixed_frame_rate;
  }


  void SceneGraph::fixed_frame_rate(Real fixed_frame_rate) {
    _fixed_frame_rate = fixed_frame_rate;
  }


  Rect SceneGraph::screen_rect() const {
    pair<int, int> window_dims = _window_adapter.window_dimensions();
    return Rect(0, 0, window_dims.first, window_dims.second);
  }


  void SceneGraph::start_frame(const Real elapsed) {
    _window_adapter.start();
    // render the game objects
    render();
  }


  void SceneGraph::render() {
    OpenVGCompanion vgc(_ovg_adapter);
  
    VGint ww = _window_adapter.window_dimensions().first;
    VGint wh = _window_adapter.window_dimensions().second;
    VGfloat black[] = {0.0,0,0,1.0};
    VGfloat red[] = {1.0,0,0,1.0};
    _ovg_adapter.vgSetfv(VG_CLEAR_COLOR, 4, black);
    _ovg_adapter.vgClear(0, 0, ww, wh);


    _ovg_adapter.vgLoadIdentity();

    for(SceneNode::iterator it = _root->begin(); it != _root->end(); ++it) {
      (*it).dispatch_render(vgc);
    }

    if(_debug_renderer) {
      //_debug_renderer->render(_world);
      for(SceneGraph::iterator it = begin(); it != end(); ++it) {
	(*it).debug_render(*_debug_renderer);
      }
    }
  }

  OpenVGAdapter& SceneGraph::vg() const {
    return _ovg_adapter;
  }


  void SceneGraph::end_frame() {
    _window_adapter.end();
  }


  void SceneGraph::add_object(SceneNode* obj) {
    _root->add_object(obj);
  }


  SceneGraph::iterator SceneGraph::begin() {
    return iterator(_root->begin(), _root->end());
  }


  SceneGraph::iterator SceneGraph::end() {
    return iterator(_root->end(), _root->end());
  }


  //===== iterator ========

  SceneGraph::iterator::iterator(wo_iterator begin, wo_iterator end) 
  {
    if(begin == end) {
      return;
    }
    _iterators.push(make_pair(begin, end));
  }


  SceneGraph::iterator::iterator(const iterator& other) {
    this->_iterators = other._iterators;
  }


  SceneGraph::iterator::~iterator() {
  }


  SceneGraph::iterator& SceneGraph::iterator::operator=(const iterator& rhs) {
    this->_iterators = rhs._iterators;
    return *this;
  }


  bool SceneGraph::iterator::operator==(const iterator& rhs) const {
    return _iterators.size() == 0 && rhs._iterators.size() == 0;
  }


  bool SceneGraph::iterator::operator!=(const iterator& rhs) const {
    return !(*this==rhs);
  }


  SceneGraph::iterator& SceneGraph::iterator::operator++() {
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
  
  SceneGraph::iterator::reference SceneGraph::iterator::operator*() const {
    return *_iterators.top().first;
  }


  SceneGraph::iterator::pointer SceneGraph::iterator::operator->() const {
  }

  // ===== iterator const =====

  // SceneGraph::const_iterator::const_iterator(bool end) 
  //   : _end(end)
  // {
  // }


  // SceneGraph::const_iterator::const_iterator(const const_iterator& other) {
  //   this->_end = other._end;
  // }


  // SceneGraph::const_iterator::~const_iterator() {
  // }


  // SceneGraph::const_iterator& SceneGraph::const_iterator::operator=(const const_iterator& rhs) {
  //   this->_end = rhs._end;
  //   return *this;
  // }


  // bool SceneGraph::const_iterator::operator==(const const_iterator& rhs) const {
  //   return _end == rhs._end;
  // }


  // bool SceneGraph::const_iterator::operator!=(const const_iterator& rhs) const {
  //   return !(*this==rhs);
  // }


  // SceneGraph::const_iterator& SceneGraph::const_iterator::operator++() {
  //   return *this;
  // }
  

  // SceneGraph::const_iterator::reference SceneGraph::const_iterator::operator*() const {
  //   return *const_cast<value_type*>(_current);
  // }


  // SceneGraph::const_iterator::pointer SceneGraph::const_iterator::operator->() const {
  // }

}; // ns::rracer
