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
    World* _world;
    bool _visible;
  public:
    HUD(const Vector& position, const Fontinfo&, OpenVGAdapter&vg, World* world, function<AffineTransform ()> world_transform);
    virtual ~HUD();

    virtual void process_input_events(const InputEventVector& events, double elapsed);
    virtual void render(OpenVGCompanion& vgc) const;
  };

}; // end ns::racer

#endif
