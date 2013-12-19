#ifndef WORLD_HH
#define WORLD_HH
#include <stack>
#include <iterator>
#include <boost/intrusive/list.hpp>
#include <Box2D/Box2D.h>

#include "common/common.hh"
#include "gfx/window-adapter.hh"
#include "gfx/openvg-adapter.hh"
#include "events/events.hh"
#include "world/world-object.hh"
#include "debug/debug.hh"

namespace rracer {

  #define WORLD_FRAMERATE 60.0
  #define WORLD_VELOCITY_ITERATIONS 6
  #define WORLD_POSITION_ITERATIONS 2


  class World {

  public:

    class iterator : public std::iterator<std::forward_iterator_tag, WorldObject> {
    public:
      typedef WorldObject::WorldObjectList::iterator wo_iterator;
      iterator(wo_iterator, wo_iterator);
      iterator(const iterator&);
      virtual ~iterator();

      iterator& operator=(const iterator&);
      bool operator==(const iterator&) const;
      bool operator!=(const iterator&) const;

      iterator& operator++();
      reference operator*() const;
      pointer operator->() const;

      Rect screen_rect() const;

    private:
      stack<pair<wo_iterator, wo_iterator> > _iterators;
    };

    // class const_iterator : public std::iterator<std::forward_iterator_tag, const WorldObject> {
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

    World(WindowAdapter &, OpenVGAdapter &);
    virtual ~World();

    iterator begin();
    iterator end();

    // const_iterator begin() const;
    // const_iterator end() const;

    void start_frame(const GameEventVector &events, Real elapsed);
    void end_frame();
    bool has_ended() { return _has_ended; };
    void add_object(WorldObject* child);

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

    WorldObject::WorldObjectList _world_objects;

    b2World* _world;

    DebugRenderer* _debug_renderer;

    Real _fixed_frame_rate;

    GameEventVector _next_frame_events;
  };

};

#endif
