#ifndef WORLD_HH
#define WORLD_HH

#include <boost/intrusive/list.hpp>
#include <Box2D/Box2D.h>

#include "common/common.hh"
#include "gfx/window-adapter.hh"
#include "gfx/openvg-adapter.hh"
#include "events/events.hh"
#include "world/world-object.hh"
#include "debug/debug.hh"


namespace rracer {

  #define WORLD_VELOCITY_ITERATIONS 6
  #define WORLD_POSITION_ITERATIONS 2

  class World {
    bool _has_ended;

    WindowAdapter &_window_adapter;
    OpenVGAdapter &_ovg_adapter;

    WorldObject::WorldObjectList _world_objects;

    b2World* _world;

    DebugRenderer* _debug_renderer;

  public:
    World(WindowAdapter &, OpenVGAdapter &);
    virtual ~World();

    void begin(const InputEventVector &events, Real elapsed);
    void end();
    bool has_ended() { return _has_ended; };

    void add_object(WorldObject* child);

    void set_debug_renderer(DebugRenderer*);
    b2World* world() const;
  };

};

#endif
