#ifndef SCENE_HH
#define SCENE_HH
#include <stack>
#include <iterator>
#include <boost/intrusive/list.hpp>
#include <Box2D/Box2D.h>

#include "common/common.hh"
#include "gfx/window-adapter.hh"
#include "gfx/openvg-adapter.hh"
#include "events/events.hh"
#include "scene/scene-node.hh"
#include "debug/debug.hh"

namespace rracer {

  #define SCENE_FRAMERATE 60.0
  #define WORLD_VELOCITY_ITERATIONS 6
  #define WORLD_POSITION_ITERATIONS 2

  class SceneRoot : public SceneNode {
  public:
    SceneRoot(function<void (SceneNode*, SceneNode*)> on_object_added_callback);
    virtual void on_object_added(SceneNode*, SceneNode*);

  private:
    function<void (SceneNode*, SceneNode*)> _on_object_added_callback;
  };


  class SceneGraph {

  public:

    class iterator : public std::iterator<std::forward_iterator_tag, SceneNode> {
    public:
      typedef SceneNode::SceneNodeList::iterator wo_iterator;
      iterator(wo_iterator, wo_iterator);
      iterator(const iterator&);
      virtual ~iterator();

      iterator& operator=(const iterator&);
      bool operator==(const iterator&) const;
      bool operator!=(const iterator&) const;

      iterator& operator++();
      reference operator*() const;
      pointer operator->() const;

    private:
      stack<pair<wo_iterator, wo_iterator> > _iterators;
    };

    // class const_iterator : public std::iterator<std::forward_iterator_tag, const SceneNode> {
    // public:
    //   const_iterator(bool);

    //   const_iterator(const const_iterator&);
    //   virtual ~const_iterator();
    //   const_iterator& operator=(const const_iterator&);

    //   bool operator==(const const_iterator&) const;
    //   bool operator!=(const const_iterator&) const;

    //   const_iterator& operator++();
    //   reference operator*() const;
    //   pointer operator->() const;
    // private:

    // };

    SceneGraph(WindowAdapter &, OpenVGAdapter &);
    virtual ~SceneGraph();

    iterator begin();
    iterator end();

    // const_iterator begin() const;
    // const_iterator end() const;

    void on_object_added(SceneNode* parent, SceneNode* child);
    void start_frame(const GameEventVector &events, Real elapsed);
    void end_frame();
    bool has_ended() { return _has_ended; };
    void add_object(SceneNode* child);

    void set_debug_renderer(DebugRenderer*);
    b2World* world() const;
    Rect screen_rect() const;

    OpenVGAdapter& vg() const;

    Real fixed_frame_rate() const;
    void fixed_frame_rate(Real fixed_frame_rate);

  private:
    void render();

    bool _has_ended;

    WindowAdapter &_window_adapter;
    OpenVGAdapter &_ovg_adapter;

    shared_ptr<SceneRoot> _root;

    b2World* _world;

    DebugRenderer* _debug_renderer;

    Real _fixed_frame_rate;

    GameEventVector _next_frame_events;

    TimeInfo _time_info;
  };

};

#endif
