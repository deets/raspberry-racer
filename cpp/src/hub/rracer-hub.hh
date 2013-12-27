#ifndef RRACER_HUB
#define RRACER_HUB

#include "debug/hud.hh"
#include "hub/hub.hh"

namespace rracer {

  class RRacerHub : public Hub {

  public:
    RRacerHub();
    virtual ~RRacerHub();

    virtual void setup(OpenVGAdapter& vg, WindowAdapter& window_adapter, fs::path bundle_path);
    virtual bool frame(Real elapsed, const GameEventVector& events);

  private:

    AssetManager* _asset_manager;
    HUD* _hud;
    SceneGraph* _scene_graph;
  };

}; // end ns::rracer

#endif