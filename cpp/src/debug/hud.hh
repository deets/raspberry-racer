#ifndef HUD_HH
#define HUD_HH

#include "world/world.hh"
#include "debug/debug.hh"

using namespace boost;

namespace rracer {
  class HUD : public WorldObject {
    DebugRenderer* _debug_renderer;
    Vector _position;
    const Fontinfo _fi;
    World& _world;
    bool _visible;
    bool _debug_renderer_set;
  public:
    HUD(const Vector& position, const Fontinfo&, World& world, function<AffineTransform ()> world_transform);
    virtual ~HUD();

    virtual void process_frame_events(const GameEventVector& events, double elapsed, EventEmitter emit_event);
    virtual void render(OpenVGCompanion& vgc) const;
  };

}; // end ns::racer

#endif
