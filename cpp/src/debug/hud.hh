#ifndef HUD_HH
#define HUD_HH

#include "scene/scene-graph.hh"
#include "debug/debug.hh"

using namespace boost;

namespace rracer {
  class HUD : public SceneNode {
    DebugRenderer* _debug_renderer;
    Vector _position;
    const Fontinfo _fi;
    SceneGraph& _scene_graph;
    bool _visible;
    bool _debug_renderer_set;
    TimeInfo _time_info;

  public:
    HUD(const Vector& position, const Fontinfo&, SceneGraph& scene_graph, function<AffineTransform ()> world_transform);
    virtual ~HUD();

    virtual void process_frame_events(const GameEventVector& events, const TimeInfo& time_info, EventEmitter emit_event);
    virtual void render(OpenVGCompanion& vgc) const;
  };

}; // end ns::racer

#endif
