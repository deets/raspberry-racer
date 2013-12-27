#ifndef RRACER_HUB
#define RRACER_HUB

#include "debug/hud.hh"
#include "hub/hub.hh"

namespace rracer {

  class PlaygroundHub : public Hub {

  public:
    PlaygroundHub();
    virtual ~PlaygroundHub();

    virtual void setup(OpenVGAdapter& vg, WindowAdapter& window_adapter, fs::path bundle_path);
    virtual bool frame(Real elapsed, const GameEventVector& events);

    static Hub* s_factory();

  private:

    AssetManager* _asset_manager;
    HUD* _hud;
    SceneGraph* _scene_graph;
  };

}; // end ns::rracer

#endif
